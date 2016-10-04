gemdos_tbase_us=E1348A
gemdos_tlen_us=62AC
gemdos_dbase_us=E3D8AA
gemdos_dlen_us=39E
vdidsp_us=E0785A
bios_tbase_us=E00000
bios_tlen_us=641C
bios_dbase_us=E35FD8
bios_dlen_us=9D8

gemdos_tbase_de=E134E4
gemdos_tlen_de=62AC
gemdos_dbase_de=E3D8DA
gemdos_dlen_de=39E
vdidsp_de=E078B4
bios_tbase_de=E00000
bios_tlen_de=6476
bios_dbase_de=E36008
bios_dlen_de=9D8

gemdos_tbase_fr=E13502
gemdos_tlen_fr=62AC
gemdos_dbase_fr=E3D8F8
gemdos_dlen_fr=39E
vdidsp_fr=E078D2
bios_tbase_fr=E00000
bios_tlen_fr=6494
bios_dbase_fr=E36026
bios_dlen_fr=9D8

gemdos_tbase_it=E134E4
gemdos_tlen_it=62AC
gemdos_dbase_it=E3D8DA
gemdos_dlen_it=39E
vdidsp_it=E078B4
bios_tbase_it=E00000
bios_tlen_it=6476
bios_dbase_it=E36008
bios_dlen_it=9D8

gemdos_tbase_uk=E1348A
gemdos_tlen_uk=62AC
gemdos_dbase_uk=E3D8AA
gemdos_dlen_uk=39E
vdidsp_uk=E0785A
bios_tbase_uk=E00000
bios_tlen_uk=641C
bios_dbase_uk=E35FD8
bios_dlen_uk=9D8

gemdos_tbase_sv=E13502
gemdos_tlen_sv=62AC
gemdos_dbase_sv=E3D8F8
gemdos_dlen_sv=39E
vdidsp_sv=E078D2
bios_tbase_sv=E00000
bios_tlen_sv=6494
bios_dbase_sv=E36026
bios_dlen_sv=9D8

gemdos_tbase_pl=3934E4
gemdos_tlen_pl=62AC
gemdos_dbase_pl=3BD8DA
gemdos_dlen_pl=39E
vdidsp_pl=3878B4
bios_tbase_pl=380000
bios_tlen_pl=6476
bios_dbase_pl=3B6008
bios_dlen_pl=9D8

gemdos_tbase=$(gemdos_tbase_$(COUNTRY))
gemdos_tlen=$(gemdos_tlen_$(COUNTRY))
gemdos_dbase=$(gemdos_dbase_$(COUNTRY))
gemdos_dlen=$(gemdos_dlen_$(COUNTRY))
bios_tbase=$(bios_tbase_$(COUNTRY))
bios_tlen=$(bios_tlen_$(COUNTRY))
bios_dbase=$(bios_dbase_$(COUNTRY))
bios_dlen=$(bios_dlen_$(COUNTRY))

rombase=$(bios_tbase)

langcheck_=false
langcheck_de=true
langcheck_us=true
langcheck_fr=true
langcheck_uk=true
langcheck_pl=true
langcheck_sv=true


#
# check that $COUNTRY is one of our supported languages
#
checklang::
	@test "$(langcheck_$(COUNTRY))" = true
