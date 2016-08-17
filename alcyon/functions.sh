#!mupfel

empty=
BS=\${empty}

RM="rm -f"

BIN="${ALCYON}\bin${BS}"
LIB="${ALCYON}\lib${BS}"
INC="${ALCYON}\include${BS}"
EXEEXT=.ttp

CFLAGS=-f 
ASFLAGS="-u -L -s ${LIB}"
CPPFLAGS="-I ${INC}"

AR=${BIN}ar68${EXEEXT}
ARFLAGS=-r
RANLIB=${BIN}doindex${EXEEXT}

LD=${BIN}aln${EXEEXT}
CRT0=${LIB}crt0.o
LDFLAGS=

RUN_OPTIMIZE_0=:
RUN_OPTIMIZE_1=${BIN}optimize${EXEEXT}
RUN_OPTIMIZE_=${RUN_OPTIMIZE_1}

AS()
{
	${BIN}as68${EXEEXT} ${ASFLAGS} $1
}

CC()
{
	f=$1
	if test "$f" = ""; then
	        echo "missing filename"
	        return 1
	fi
	if test ! -f $f.c; then
	        echo "$f.c: no such file"
	        return 1
	fi
	${BIN}cp68${EXEEXT} ${CPPFLAGS} $f.c $f.i
	${BIN}c068${EXEEXT} $f.i $f.1 $f.2 $f.3 ${CFLAGS}
	${BIN}c168${EXEEXT} $f.1 $f.2 $f.s
	AS $f.s
	eval OPTIMIZE=${f}_OPTIMIZE
	eval OPTIMIZE="$"${OPTIMIZE}
	eval OPTIMIZE=RUN_OPTIMIZE_${OPTIMIZE}
	eval OPTIMIZE="$"${OPTIMIZE}
	$OPTIMIZE $f.s > NUL:
	${RM} $f.s $f.i $f.1 $f.2 $f.3
}
