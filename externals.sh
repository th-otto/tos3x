#!/bin/sh

eval `cat bios/SRCFILES vdi/SRCFILES bdos/SRCFILES aes/SRCFILES desk/SRCFILES | sed -e '/\\\\$/d' -e '/^#/d' -e 's/\${TOSVERSION}/306/' -e 's/^\(.*\)=\(.*\)$/\1="\2"/' -e '/^$/d' -e '/\${empty}/d' -e 's/$/;/'`


set -- ${BIOS_OBJS}
BIOS_OBJS="${@/#/bios/}"
set -- ${VDI_OBJS}
VDI_OBJS="${@/#/vdi/}"
set -- ${VDOS_OBJS}
BDOS_OBJS="${@/#/bdos/}"
set -- ${AES_OBJS}
AES_OBJS="${@/#/aes/}"
set -- ${DESK_OBJS}
DESK_OBJS="${@/#/desk/}"

OBJS="${BIOS_OBJS} ${VDI_OBJS} ${BDOS_OBJS} ${AES_OBJS} ${DESK_OBJS}"

duplicates=`nm -A -g ${OBJS} 2>/dev/null | grep ' [TDBC] ' | sed -e 's/:[0-9a-f]* / /' | cut -d ' ' -f 3 | cut -c 1-8 | sort | uniq -d`
echo duplicates=$duplicates
