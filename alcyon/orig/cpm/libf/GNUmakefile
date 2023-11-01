EVAL_CROSS := case `uname -s` in *MiNT*|*TOS*) echo no;; *) echo yes; esac
CROSS := $(shell $(EVAL_CROSS))
top_srcdir=../..

include $(top_srcdir)/GNUmakefile.cmn
include $(top_srcdir)/Makefile.sil

CPPFLAGS = -I$(INC)
RUN_OPTIMIZE_1 = 

LIBF_OBJS = \
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
	ffplog.o \
	ffpmul2.o \
	ffppwr.o \
	ffpsin.o \
	ffpsqrt.o \
	ffptheta.o \
	ffptnorm.o \
	ffptof.o \
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
	ftoffp.o \
	ftol.o \
	ltof.o \
	fpltof.o \
	xdoprtfp.o \
	\
	atan.o \
	ffpatan.o \
	cosh.o \
	sinh.o \
	tanh.o \
	ffpsinh.o \
	fpftol.o \
	$(empty)

all: libf.a

libf.a: $(LIBF_OBJS)
	$(AM_V_at)$(RM) $@
	$(AM_V_AR)$(AR) $(ARFLAGS) $@ ${LIBF_OBJS}

clean distclean::
	$(RM) *.o *.a
