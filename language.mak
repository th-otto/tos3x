LANG=de
TOSVERSION=306

gemdos_tbase_de=E134E4
gemdos_tlen_de=62AC
gemdos_dbase_de=E3D8DA
gemdos_dlen_de=39E
vdidsp_de=E078B4

gemdos_tbase_us=E1348A
gemdos_tlen_us=62AC
gemdos_dbase_us=E3D8AA
gemdos_dlen_us=39E
vdidsp_us=E0785A

gemdos_tbase_fr=E13502
gemdos_tlen_fr=62AC
gemdos_dbase_fr=E3D8F8
gemdos_dlen_fr=39E
vdidsp_fr=E078D2

gemdos_tbase_uk=E1348A
gemdos_tlen_uk=62AC
gemdos_dbase_uk=E3D8AA
gemdos_dlen_uk=39E
vdidsp_uk=E0785A

gemdos_tbase_pl=3934E4
gemdos_tlen_pl=62AC
gemdos_dbase_pl=3BD8DA
gemdos_dlen_pl=39E
vdidsp_pl=3878B4

gemdos_tbase=$(gemdos_tbase_$(LANG))
gemdos_dbase=$(gemdos_dbase_$(LANG))

langcheck_=false
langcheck_de=true
langcheck_us=true
langcheck_fr=true
langcheck_uk=true
langcheck_pl=true


#
# check that $LANG is one of our supported languages
#
checklang::
	@test "$(langcheck_$(LANG))" = true
