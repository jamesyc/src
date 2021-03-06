/*	$OpenBSD: rthread_fork.c,v 1.19 2016/09/04 10:13:35 akfaew Exp $ */

/*
 * Copyright (c) 2008 Kurt Miller <kurt@openbsd.org>
 * Copyright (c) 2008 Philip Guenther <guenther@openbsd.org>
 * Copyright (c) 2003 Daniel Eischen <deischen@freebsd.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Neither the name of the author nor the names of any co-contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $FreeBSD: /repoman/r/ncvs/src/lib/libc_r/uthread/uthread_atfork.c,v 1.1 2004/12/10 03:36:45 grog Exp $
 */

#ifndef NO_PIC
#include <sys/types.h>
#include <sys/exec_elf.h>
#pragma weak _DYNAMIC
#endif

#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <tib.h>
#include <unistd.h>

#include "thread_private.h"	/* in libc/include */

#include "rthread.h"
#include "rthread_cb.h"

/* make {fork,vfork,getthrid} call _thread_sys_{fork,vfork,getthrid} */
REDIRECT_SYSCALL(fork);
REDIRECT_SYSCALL(vfork);
REDIRECT_SYSCALL(getthrid);

static pid_t
_dofork(pid_t (*sys_fork)(void))
{
	pthread_t me;
	pid_t newid;
	int i;

	if (!_threads_ready)
		return sys_fork();

	me = pthread_self();

	/*
	 * Protect important libc/ld.so critical areas across the fork call.
	 * dlclose() will grab the atexit lock via __cxa_finalize() so lock
	 * the dl_lock first. malloc()/free() can use arc4random(), so lock
	 * malloc_lock before arc4_lock
	 */

#ifndef NO_PIC
	if (_DYNAMIC)
		_rthread_dl_lock(0);
#endif

	_thread_atexit_lock();
	for (i = 0; i < _MALLOC_MUTEXES; i++)
		_thread_malloc_lock(i);
	_thread_arc4_lock();

	newid = sys_fork();

	_thread_arc4_unlock();
	if (newid == 0)
		_thread_malloc_reinit();
	else
		for (i = 0; i < _MALLOC_MUTEXES; i++)
			_thread_malloc_unlock(i);
	_thread_atexit_unlock();

	if (newid == 0) {
		struct tib *tib = me->tib;
#ifndef NO_PIC
		/* reinitialize the lock in the child */
		if (_DYNAMIC)
			_rthread_dl_lock(2);
#endif
		/* update this thread's structure */
		tib->tib_tid = getthrid();
		me->donesem.lock = _SPINLOCK_UNLOCKED;
		me->flags_lock = _SPINLOCK_UNLOCKED;

		/* reinit the thread list */
		LIST_INIT(&_thread_list);
		LIST_INSERT_HEAD(&_thread_list, me, threads);
		_thread_lock = _SPINLOCK_UNLOCKED;

		/* single threaded now */
		__isthreaded = 0;
	}
#ifndef NO_PIC
	else if (_DYNAMIC)
		_rthread_dl_lock(1);
#endif
	return newid;
}

pid_t
_thread_fork(void)
{
	return _dofork(&fork);
}

pid_t
_thread_vfork(void)
{
	return _dofork(&vfork);
}
