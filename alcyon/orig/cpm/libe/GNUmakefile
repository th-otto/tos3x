EVAL_CROSS := case `uname -s` in *MiNT*|*TOS*) echo no;; *) echo yes; esac
CROSS := $(shell $(EVAL_CROSS))
top_srcdir=../..

include $(top_srcdir)/GNUmakefile.cmn
include $(top_srcdir)/Makefile.sil

CFLAGS += -mieee

CPPFLAGS = -I$(INC)

LIBE_OBJS = \
	atof.o \
	ceil.o \
	etoa.o \
	fabs.o \
	ffpabs.o \
	ffpadd.o \
	ffpcmp.o \
	ffpcpyrt.o \
	ffpdiv.o \
	ffpexp.o \
	ffphthet.o \
	ffpieee.o \
	ffplog.o \
	ffpmul2.o \
	ffppwr.o \
	ffpsin.o \
	ffpsqrt.o \
	ffptheta.o \
	ffptnorm.o \
	floor.o \
	fmod.o \
	fpadd.o \
	fpcmp.o \
	fpcos.o \
	fpdiv.o \
	fpexp.o \
	fplog.o \
	fpmul.o \
	fpneg.o \
	fppwr.o \
	fpsin.o \
	fpsqrt.o \
	fpsub.o \
	ftoa.o \
	ftol.o \
	ieeetof.o \
	iefabs.o \
	iefadd.o \
	iefback.o \
	iefcmp.o \
	iefdiv.o \
	iefexp.o \
	ieffpi.o \
	ieffront.o \
	iefifp.o \
	ieflog.o \
	iefmul.o \
	iefpwr.o \
	iefsin.o \
	iefsqrt.o \
	ltof.o \
	xdoprtfp.o \
	\
	atan.o \
	ffpatan.o \
	iefsinh.o \
	iefatan.o \
	cosh.o \
	sinh.o \
	tanh.o \
	fpftol.o \
	fptoieee.o \
	$(empty)

all: libe.a

libe.a: $(LIBE_OBJS) ${MAKEFILE}
	$(AM_V_at)$(RM) $@
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ ${LIBE_OBJS}

clean distclean::
	$(RM) *.o *.a
