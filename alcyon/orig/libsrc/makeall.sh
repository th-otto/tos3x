#!sh

. ..\functions.sh

. srcfiles.sh
. optimize.mak

for i in ${AOBJS1} ${AOBJS2} ${CRT0}; do
	i=`basename $i .o`.S
	echo AS $i
	AS $i
done

for i in ${COBJS1} ${COBJS2} ${COBJS3} ${COBJS4} ${COBJS5} ${COBJS6} ${COBJS7} ${COBJS8} ${COBJS9} ${COBJS10} ${COBJS11} ${COBJS12} ${COBJS13} ${COBJS14} ${COBJS15} ${COBJS16} ${COBJS17}; do
	i=`basename $i .o`
	echo CC $i.c
	CC $i
done

$RM ${LIBC}
echo AR ${LIBC}
${AR} ${ARFLAGS} ${LIBC} ${AOBJS1}
${AR} ${ARFLAGS} ${LIBC} ${AOBJS2}
${AR} ${ARFLAGS} ${LIBC} ${COBJS1}
${AR} ${ARFLAGS} ${LIBC} ${COBJS2}
${AR} ${ARFLAGS} ${LIBC} ${COBJS3}
${AR} ${ARFLAGS} ${LIBC} ${COBJS4}
${AR} ${ARFLAGS} ${LIBC} ${COBJS5}
${AR} ${ARFLAGS} ${LIBC} ${COBJS6}
${AR} ${ARFLAGS} ${LIBC} ${COBJS7}
${AR} ${ARFLAGS} ${LIBC} ${COBJS8}
${AR} ${ARFLAGS} ${LIBC} ${COBJS9}
${AR} ${ARFLAGS} ${LIBC} ${COBJS10}
${AR} ${ARFLAGS} ${LIBC} ${COBJS11}
${AR} ${ARFLAGS} ${LIBC} ${COBJS12}
${AR} ${ARFLAGS} ${LIBC} ${COBJS13}
${AR} ${ARFLAGS} ${LIBC} ${COBJS14}
${AR} ${ARFLAGS} ${LIBC} ${COBJS15}
${AR} ${ARFLAGS} ${LIBC} ${COBJS16}
${AR} ${ARFLAGS} ${LIBC} ${COBJS17}
echo RANLIB ${LIBC}
${RANLIB} ${LIBC}
