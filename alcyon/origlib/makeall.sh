#!sh

. ..\functions.sh

. srcfiles.sh
. optimize.mak

for i in $AOBJS1 $AOBJS2 $CRT0O; do
	i=`basename $i .o`.S
	echo AS $i
	AS $i
done

for i in $COBJS1 $COBJS2 $COBJS3 $COBJS4 $COBJS5 $COBJS6 $COBJS7 $COBJS8; do
	i=`basename $i .o`
	echo CC $i.c
	CC $i
done

$RM $LIBC
echo AR $LIBC
${AR} ${ARFLAGS} $LIBC ${AOBJS1}
${AR} ${ARFLAGS} $LIBC ${AOBJS2}
${AR} ${ARFLAGS} $LIBC ${COBJS1}
${AR} ${ARFLAGS} $LIBC ${COBJS2}
${AR} ${ARFLAGS} $LIBC ${COBJS3}
${AR} ${ARFLAGS} $LIBC ${COBJS4}
${AR} ${ARFLAGS} $LIBC ${COBJS5}
${AR} ${ARFLAGS} $LIBC ${COBJS6}
${AR} ${ARFLAGS} $LIBC ${COBJS7}
${AR} ${ARFLAGS} $LIBC ${COBJS8}
echo RANLIB $LIBC
${RANLIB} $LIBC
