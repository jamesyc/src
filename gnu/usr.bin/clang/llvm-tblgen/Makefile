# $OpenBSD: Makefile,v 1.5 2017/04/17 19:58:53 pascal Exp $

PROG=	llvm-tblgen
NOMAN=

.include <bsd.own.mk>
SRCS=	AsmMatcherEmitter.cpp \
	AsmWriterEmitter.cpp \
	AsmWriterInst.cpp \
	Attributes.cpp \
	CallingConvEmitter.cpp \
	CodeEmitterGen.cpp \
	CodeGenDAGPatterns.cpp \
	CodeGenInstruction.cpp \
	CodeGenMapTable.cpp \
	CodeGenRegisters.cpp \
	CodeGenSchedule.cpp \
	CodeGenTarget.cpp \
	DAGISelEmitter.cpp \
	DAGISelMatcherEmitter.cpp \
	DAGISelMatcherGen.cpp \
	DAGISelMatcherOpt.cpp \
	DAGISelMatcher.cpp \
	DFAPacketizerEmitter.cpp \
	DisassemblerEmitter.cpp \
	FastISelEmitter.cpp \
	FixedLenDecoderEmitter.cpp \
	GlobalISelEmitter.cpp \
	InstrInfoEmitter.cpp \
	IntrinsicEmitter.cpp \
	OptParserEmitter.cpp \
	PseudoLoweringEmitter.cpp \
	RegisterInfoEmitter.cpp \
	SearchableTableEmitter.cpp \
	SubtargetEmitter.cpp \
	SubtargetFeatureInfo.cpp \
	TableGen.cpp \
	Types.cpp \
	X86DisassemblerTables.cpp \
	X86ModRMFilters.cpp \
	X86RecognizableInstr.cpp \
	CTagsEmitter.cpp

.PATH:	${.CURDIR}/../../../llvm/utils/TableGen

LLVM_LIBDEPS=	LLVMTableGen LLVMSupport

depend:
	# Nothing here so far ...

install:
	# Nothing here so far ...

.include <bsd.prog.mk>
