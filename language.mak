bios_tbase_us=E00000
bios_tlen_us=641C
bios_dbase_us=E35FD8
bios_dlen_us=9D8
vdi_tbase_us=E0641C
vdi_tlen_us=D06E
vdi_dbase_us=E369B0
vdi_dlen_us=6EFA
vdidsp_us=E0785A
gemdos_tbase_us=E1348A
gemdos_tlen_us=62AC
gemdos_dbase_us=E3D8AA
gemdos_dlen_us=39E

bios_tbase_de=E00000
bios_tlen_de=6476
bios_dbase_de=E36008
bios_dlen_de=9D8
vdi_tbase_de=E06476
vdi_tlen_de=D06E
vdi_dbase_de=E369E0
vdi_dlen_de=6EFA
vdidsp_de=E078B4
gemdos_tbase_de=E134E4
gemdos_tlen_de=62AC
gemdos_dbase_de=E3D8DA
gemdos_dlen_de=39E

bios_tbase_fr=E00000
bios_tlen_fr=6494
bios_dbase_fr=E36026
bios_dlen_fr=9D8
vdi_tbase_fr=E06494
vdi_tlen_fr=D06E
vdi_dbase_fr=E369FE
vdi_dlen_fr=6EFA
vdidsp_fr=E078D2
gemdos_tbase_fr=E13502
gemdos_tlen_fr=62AC
gemdos_dbase_fr=E3D8F8
gemdos_dlen_fr=39E

bios_tbase_es=E00000
bios_tlen_es=64A2
bios_dbase_es=E36034
bios_dlen_es=9D8
vdi_tbase_es=E064A2
vdi_tlen_es=D06E
vdi_dbase_es=E36A0C
vdi_dlen_es=6EFA
vdidsp_es=E078E0
gemdos_tbase_es=E134F2
gemdos_tlen_es=62AC
gemdos_dbase_es=E3D8E8
gemdos_dlen_es=39E

bios_tbase_it=E00000
bios_tlen_it=6484
bios_dbase_it=E36016
bios_dlen_it=9D8
vdi_tbase_it=E06484
vdi_tlen_it=D06E
vdi_dbase_it=E369EE
vdi_dlen_it=6EFA
vdidsp_it=E078C2
gemdos_tbase_it=E13602
gemdos_tlen_it=62AC
gemdos_dbase_it=E3D8E8
gemdos_dlen_it=39E

bios_tbase_uk=E00000
bios_tlen_uk=641C
bios_dbase_uk=E35FD8
bios_dlen_uk=9D8
vdi_tbase_uk=E0641C
vdi_tlen_uk=D06E
vdi_dbase_uk=E369B0
vdi_dlen_uk=6EFA
vdidsp_uk=E0785A
gemdos_tbase_uk=E1348A
gemdos_tlen_uk=62AC
gemdos_dbase_uk=E3D8AA
gemdos_dlen_uk=39E

bios_tbase_sv=E00000
bios_tlen_sv=6494
bios_dbase_sv=E36026
bios_dlen_sv=9D8
vdi_tbase_sv=E06494
vdi_tlen_sv=D06E
vdi_dbase_sv=E369FE
vdi_dlen_sv=6EFA
vdidsp_sv=E078D2
gemdos_tbase_sv=E13502
gemdos_tlen_sv=62AC
gemdos_dbase_sv=E3D8F8
gemdos_dlen_sv=39E

bios_tbase_pl=380000
bios_tlen_pl=6476
bios_dbase_pl=3B6008
bios_dlen_pl=9D8
vdi_tbase_pl=386476
vdi_tlen_pl=D06E
vdi_dbase_pl=3B69E0
vdi_dlen_pl=6EFA
vdidsp_pl=3878B4
gemdos_tbase_pl=3934E4
gemdos_tlen_pl=62AC
gemdos_dbase_pl=3BD8DA
gemdos_dlen_pl=39E

bios_tbase=$(bios_tbase_$(COUNTRY))
bios_tlen=$(bios_tlen_$(COUNTRY))
bios_dbase=$(bios_dbase_$(COUNTRY))
bios_dlen=$(bios_dlen_$(COUNTRY))
vdi_tbase=$(vdi_tbase_$(COUNTRY))
vdi_tlen=$(vdi_tlen_$(COUNTRY))
vdi_dbase=$(vdi_dbase_$(COUNTRY))
vdi_dlen=$(vdi_dlen_$(COUNTRY))
gemdos_tbase=$(gemdos_tbase_$(COUNTRY))
gemdos_tlen=$(gemdos_tlen_$(COUNTRY))
gemdos_dbase=$(gemdos_dbase_$(COUNTRY))
gemdos_dlen=$(gemdos_dlen_$(COUNTRY))

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
