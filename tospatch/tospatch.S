/********************************************************************************
 * TosPatch V2.4:                                                               *
 * Dieses Programm dient zum Patchen des TOS mit einem einfachen ASCII-File.    *
 *                                                                              *
 * (C)1990    Sigma-soft, Markus Fritze                                         *
 * (C)1992-93 Markus Heiden                                          19.09.1993 *
 ********************************************************************************/

/* Versionsnummer */
#define version  '2.4'

def_base        EQU $00E00000   ;Default-Basisadresse des TOS
def_length      EQU $00040000   ;Default-Laenge des TOS
max_length      EQU $00100000   /* Die maximale Laenge eines zu bearbeitenden TOS */
def_width       EQU 16          ;Default-Breite des TOS

stack_size      EQU 4096        ;Stack fuer das Programm

                TEXT

#if 0
#define PCRD(x) (__etext+(x-__data)).w(pc)
#define PCRB(x) (__etext+__sdata+(x-__bss)).w(pc)
#else
#define PCRD(x) x(pc)
#define PCRB(x) x(pc)
#endif

init:           movea.l 4(SP),A6        ;Basepageadresse holen
                movea.w #$0100,A5       ;Groesse der Basepage
                adda.l  12(A6),A5       ;+ Groesse des TEXT-Segments
                adda.l  20(A6),A5       ;+ Groesse des DATA-Segments
                adda.l  28(A6),A5       ;+ Groesse des BSS-Segments

                move.l  A5,-(SP)        ;Programmlaenge
                move.l  A6,-(SP)        ;Adresse der Basepage
                move.l  #$004A0000,-(SP) ;Funktionsnummer + Dummyword (0)
                trap    #1              ;Mshrink(0,Basepageadr,Prglaenge)

                lea     stack+stack_size,SP ;eigenen Stack setzen

                lea     PCRD(init_text),A0
                bsr     print_line

                lea     buffer,A0       ;TOS-Puffer loeschen
                moveq   #-1,D0          ;mit $FF
                move.l  #(max_length/16),D1
init1:          move.l  D0,(A0)+
                move.l  D0,(A0)+
                move.l  D0,(A0)+
                move.l  D0,(A0)+
                subq.l  #1,D1
                bne.s   init1

                lea     PCRD(read_text),A0
                bsr     print_line

                lea     128(A6),A0      ;Zeiger auf die Commandline
                moveq   #0,D0
                move.b  (A0)+,D0
                clr.b   0(A0,D0.w)      ;Commandline mit einem Nullbyte abschliessen

                tst.b   (A0)            ;Filename angegeben?
                bne.s   init2           ;Ja! => den dann nehmen
                lea     PCRD(bfname),A0   ;sonst "PATCH.FIL"
init2:          lea     PCRB(btchbuf),A1
                bsr     read_length     ;Laenge des Files holen
                cmp.l   #$00010000,D0   ;Ist es zu lang?
                bhs     error6          ;Ja! =>
                bsr     read_file       ;Batch-File einlesen

                bsr     mkplis          ;ASCII-Datei auswerten

                bsr     patch_it        ;TOS patchen...

                bsr     reloc_it        ;TOS ggf. relozieren...

                bsr     crc_checksum    ;Pruefsumme ggf. setzen...

                bsr.s   save_tos        ;und das TOS wieder abspeichern

exit:           pea     PCRD(key_text)    ;Taste...
                move.w  #9,-(SP)        ;Cconws
                trap    #1              ;GEMDOS 9
                addq.l  #6,SP

                move.w  #7,-(SP)
                trap    #1              ;auf Taste warten
                addq.l  #2,SP

quit:           clr.w   -(SP)
                trap    #1              ;Pterm0()

                illegal                 ;Abbruch falls GEMDOS doch zurueckkehrt.

****************************************
* save_tos: Speichert das TOS-File
* IN length.l       : Laenge des TOS
*    output_format.l: Anzahl der zu speichernden Teildateien
*    width.l        : Breite des TOS

save_tos:       lea     PCRD(write_tos),A0 ;'Schreibe TOS'
                bsr     print_line      ;ausgeben

                move.l  PCRB(output_format),D2 ;D2 = Anzahl der Files
                beq     savtos_end      ;kein File? Ja! =>
                cmp.l   #32,D2          ;Zu viele Files?
                bhi     error12         ;Ja! =>
                cmp.l   #1,D2           ;Nur ein Ausgabefile?
                bne.s   savtos1         ;Nein! =>

                move.l  PCRB(length),D0   ;TOS in einem Stueck abspeichern
                lea     PCRD(fname),A0
                lea     buffer,A1
                bra     write_file

savtos1:        move.l  PCRB(width),D1
                move.l  D1,D7
                add.l   D7,D7
                cmp.l   D7,D2           ;mehr als 16 Baenke?
                bhi     error12         ;Ja! =>

                lsr.l   #3,D1           ;D1 = Offset
                move.l  D1,D7           ;(WIDTH/8)
                subq.l  #1,D7
                and.l   D2,D7           ;Passt WIDTH und Anzahl der Files zusammen?
                bne     error21         ;Nein! =>

                move.w  PCRB(length),D7   ;Laenge eines Files berechnen
                divu    D2,D7           ;(LENGTH/OUTPUT_FORMAT)
                moveq   #0,D0
                move.w  D7,D0
                swap    D0
                move.w  PCRB(length+2),D7
                divu    D2,D7
                move.w  D7,D0           ;D0 = Laenge eines Files
                swap    D7
                tst.w   D7
                bne     error23

                move.l  D1,D6           ;log 2 (WIDTH/8)
                moveq   #0,D7
savtos2:        lsr.w   #1,D6
                beq.s   savtos3
                addq.l  #1,D7
                bra.s   savtos2

savtos3:        asl.l   #2,D7           ;auf Longword-Offset

                lea     PCRD(w_tab),A5    ;Adresse der Extensiontabelle berechnen
                movea.l 0(A5,D7.l),A5
                lea     buffer,A6

                moveq   #$30,D5         ;Nummer fuer TOS-Dateien
                clr.l   fname_ext       ;File-Extension erstmal loeschen

savtos4:        moveq   #0,D7           ;Offset-Zaehler
                moveq   #0,D6           ;Laengen-Zaehler

savtos5:        lea     0(A6,D7.l),A2   ;Anfangsadresse berechnen
                lea     patch_list,A3   ;Zwischenspeicher fuer File

                move.l  D0,D4

savtos6:        move.b  (A2),(A3)+      ;TOS in Zwischenspeicher kopieren
                adda.l  D1,A2
                subq.l  #1,D4
                bne.s   savtos6

                add.l   D0,D6           ;Laenge aufsummieren

                move.l  D7,D4
                add.l   D4,D4           ;Offset auf Word
                move.w  0(A5,D4.l),D4   ;Endung holen
                move.w  D4,fname_ext    ;und schreiben

                cmp.l   PCRB(output_format),D1 ;Nur eine Bank?
                beq.s   savtos7         ;Ja! =>
                move.b  D5,fname_ext+2  ;sonst Nummer in Extension eintragen

savtos7:        lea     PCRD(fname),A0    ;File schreiben
                lea     patch_list,A1
                bsr     write_file

                addq.l  #1,D7           ;naechstes File
                cmp.l   D1,D7           ;schon eine Bank fertig?
                blo.s   savtos5         ;Nein! =>

                adda.l  D6,A0           ;Adresse auf naechste Bank
                addq.b  #1,D5           ;Nummer der TOS-Dateien erhoehen
                cmp.b   #'9'+1,D5
                bne.s   savtos8
                addq.b  #'A'-('9'+1),D5

savtos8:        sub.l   D1,D2           ;schon alle Files?
                bne.s   savtos4         ;Nein! =>

savtos_end:     rts

mkplis:         moveq   #0,D0           ;'CRC' erzeugen und auf 0 setzen
                move.l  #$43524300,var_name /* 'CRC' */
                move.l  D0,var_name+4
                bsr     write_var

                lea     PCRB(btchbuf),A6
                lea     patch_list,A5

mkplis0:        move.l  #1,line         ;Zeilennummer initialisieren
                clr.w   if_count        ;Bedingungszaehler initialisieren
mkplis1:        bsr     skip_space
                bne.s   mkplis2         ;Zeilenende? Ja! =>
                cmp.b   #'!',D1
                beq     mkplis11
                cmp.b   #'?',D1
                beq     mkplis27
                cmp.b   #'=',D1
                beq     mkplis14
                cmp.b   #'>',D1
                beq     mkplis21
                cmp.b   #'<',D1
                beq     mkplis17
                cmp.b   #'-',D1
                beq     mkplis23
                cmp.b   #'+',D1
                beq     mkplis31
                cmp.b   #$dd,D1
                beq     mkplis26
                cmp.b   #'#',D1
                beq     mkplis30
                cmp.b   #'.',D1
                beq     mkplis33
                cmp.b   #'/',D1
                beq     mkplis36
                cmp.b   #'_',D1
                beq     mkplis41
                cmp.b   #' ',D1
                bls.s   mkplis2
                cmp.b   #';',D1
                bne.s   mkplis3

****************************************
* Naechsten Zeilenanfang suchen

mkplis2:        move.b  (A6)+,D1        ;naechste Zeile suchen
                beq     mkplis25        ;Datei-Ende? Ja! =>
                cmp.b   #$0A,D1         ;naechste Zeile?
                bne.s   mkplis2         ;Nein! =>
                addq.l  #1,line         ;Zeilennummer um 1 erhoehen
                bra     mkplis1


****************************************
* Patches auswerten

mkplis3:        bsr     get_term        ;Patchoffset holen
                move.l  D1,D7
                bsr     get_length      ;Laenge des TOS holen
                cmp.l   D1,D7           ;Patchoffset < Laenge?
                blo.s   mkplis5         ;Ja! => weiter
                move.l  D1,D6
                bsr     get_base        ;TOS-Basisadresse holen
                sub.l   A1,D7
                cmp.l   D6,D7           ;war Patchoffset absolut angegeben?
                bhs     error1          ;Nein! =>

mkplis5:        cmpi.b  #':',(A6)       ;~ setzen?
                bne.s   mkplis5d        ;Nein! => weiter

                addq.l  #1,A6
                move.l  D7,current_adr  ;~ auf Patchoffset setzen
                bra     mkplis1

mkplis5d:       bsr     skip_space
                bne     error0
                move.l  D7,-(SP)

                moveq   #0,D0           ;Flag loeschen

                cmpi.b  #'|',D1         ;~ nicht veraendern?
                bne.s   mkplis5x        ;Nein! =>

                addq.l  #1,A6
                moveq   #1,D0           ;Flag setzen

mkplis5x:       move.w  D0,on_off       ;Flag fuer ~ veraendern setzen

                cmpi.b  #'#',(A6)
                beq.s   mkplis5a
                cmpi.b  #'.',(A6)
                beq.s   mkplis5a
                cmpi.b  #'$',(A6)
                bne.s   mkplis8

****************************************
* Einfache Patches lesen

                addq.l  #1,A6
mkplis5a:       moveq   #0,D7           ;Laenge des Patches auf 0 setzen
                lea     8(A5),A1        ;Adresse fuer die Patchdaten setzen
mkplis5b:       cmpi.b  #'#',(A6)       ;Einfache Variable?
                beq.s   mkplis7a        ;Ja! =>
                bsr     skip_space
                bne     error0
                bsr     get_term        ;Patchwert
                bsr     get_extension   ;und dessen Breite holen
mkplis5c:       add.l   D2,D7           ;Breite zur Laenge
                adda.l  D2,A1           ;und zur Adresse addieren
                movea.l A1,A0
mkplis6:        move.b  D1,-(A0)        ;Patch in Tabelle schreiben
                lsr.l   #8,D1
                subq.l  #1,D2
                bne.s   mkplis6

mkplis7:        cmpi.b  #',',(A6)+      ;noch ein Patchwert?
                beq.s   mkplis5b        ;Ja! =>
                bra.s   mkpli10         ;Patch abschliessen

mkplis7a:       move.l  PCRB(variable),D1 ;Einfache Variable holen
                addq.l  #1,A6           ;'#' ueberspringen
                move.b  (A6)+,D0        ;Extension holen
                and.b   #$DF,D0         ;Auf Grossschrift wandeln
                moveq   #1,D2           ;Schon mal auf Byte setzen
                cmpi.b  #'B',D0         ;Ist es Byte?
                beq.s   mkplis5c        ;Ja! =>
                moveq   #2,D2           ;Schon mal auf Word setzen
                cmpi.b  #'W',D0         ;Ist es Word?
                beq.s   mkplis5c        ;Ja! =>
                moveq   #3,D2           ;Schon mal auf Long setzen
                cmpi.b  #'L',D0         ;Ist es Long?
                beq.s   mkplis5c        ;Ja! =>
                bra     error29         ;Sonst => Fehler

****************************************
* Patches in Fileform laden

mkplis8:        movea.l A6,A0           ;Zeiger auf Filename
mkplis9:        cmpi.b  #' ',(A6)+      ;Filename-Ende suchen
                bhi.s   mkplis9
                clr.b   -1(A6)          ;und mit Nullbyte abschliessen

                lea     8(A5),A1        ;Adresse fuer die Patchdaten setzen

                move.l  #patch_list,D0  ;max. Laenge berechnen
                add.l   #max_length/2-16,D0
                sub.l   A1,D0
                bmi     error27         ;Kein Platz mehr? Ja! =>
                bsr     read_maxfile    ;Patch einlesen
                bmi     error27         ;Patch zu lang? Ja! =>

                move.l  D0,D7           ;Laenge in richtiges Register

mkpli10:        move.l  (SP)+,D6        ;Patchoffset holen

                tst.w   on_off          ;~ aktualisieren?
                bne.s   mkpli10a        ;Nein! =>

                move.l  D6,D0           ;Patchoffset
                add.l   D7,D0           ;plus Patchlaenge
                move.l  D0,current_adr  ;gleich aktuelle Adresse

mkpli10a:       move.l  D7,(A5)+        ;Patchlaenge und
                move.l  D6,(A5)+        ;Patchoffset in Liste schreiben
                adda.l  D7,A5
                clr.b   (A5)+           ;EVEN
                move.l  A5,D0
                and.b   #$FE,D0
                movea.l D0,A5
                clr.l   (A5)            ;Liste mit 0 abschliessen
                bra     mkplis2


****************************************
* Relocausnahmen auswerten

mkplis11:       addq.l  #1,A6
                lea     PCRB(ptchtab),A0 ;Ausnahmentabellenende suchen
mkplis12:       tst.l   (A0)+
                bne.s   mkplis12
                subq.l  #4,A0
                lea     PCRB(ptchtab2),A1 ;Feldausnahmentabellenende suchen
mkpli12a:       tst.l   (A1)
                addq.l  #8,A1
                bne.s   mkpli12a
                subq.l  #8,A1
mkplis13:       bsr     skip_space
                bne     error2
                bsr     get_term        ;Adresse holen
                move.l  D1,(A0)+        ;und in Tabelle sichern
                bsr     skip_space
                bne     mkplis2
mkpli13a:       cmpi.b  #',',(A6)+      ;noch ein Wert?
                beq.s   mkplis13        ;Ja! =>
                cmpi.b  #':',D1         ;Feldausnahme?
                beq.s   mkpli13b        ;Ja! =>
                cmpi.b  #'.',D1         ;Feldausnahme?
                bne     error2          ;Nein! =>
                cmpi.b  #'.',(A6)+      ;2. Zeichen pruefen
                bne     error2          ;Fehler? Ja! =>

mkpli13b:       bsr     skip_space
                bne     error0
                bsr     get_term
                cmp.l   -4(A0),D1       ;Ist 2. Adresse groesser als die 1.?
                blo     error28         ;Nein! =>
                beq.s   mkpli13c        ;Bei Gleichheit Wert lassen
                move.l  -(A0),(A1)+     ;Anfangsadresse in richtige Tabelle
                clr.l   (A0)            ;und in der falschen Tabelle loeschen
                move.l  D1,(A1)+        ;Endadresse schreiben
mkpli13c:       bsr     skip_space
                bne     mkplis2
                cmpi.b  #',',(A6)+      ;noch ein Wert?
                beq.s   mkplis13        ;Ja! =>
                bra     error2


****************************************
* TOS-Datum vergleichen

mkplis14:       addq.l  #1,A6
                moveq   #0,D1
                bsr.s   mkplis16        ;Tag holen
                bsr.s   mkplis16
                cmpi.b  #'.',(A6)+
                bne     error26
                bsr.s   mkplis16        ;Monat holen
                bsr.s   mkplis16
                ror.w   #8,D1           ;Tag und Monat tauschen (US-Format)
                cmpi.b  #'.',(A6)+
                bne     error26
                bsr.s   mkplis16        ;Jahr holen
                bsr.s   mkplis16
                bsr.s   mkplis16
                bsr.s   mkplis16
                lea     buffer+$0018,A0 ;Adresse des Datums im TOS
                cmp.l   (A0),D1         ;Ist es gleich dem angegebenen?
                beq     mkplis2         ;Ja! =>
                bsr     error14
                bra     mkplis2

mkplis16:       lsl.l   #4,D1
                move.b  (A6)+,D0
                eori.b  #'0',D0
                cmp.b   #9,D0
                bhi     error26
                or.b    D0,D1
                rts


****************************************
* TOS lesen

mkplis17:   addq.l  #1,A6
                cmpi.b  #'*',(A6)       ;TOS aus ROM lesen?
                bne.s   mkplis19        ;Nein! =>

****************************************
* TOS aus ROM lesen

                lea     PCRD(read_rom_text),A0
                bsr     print_line

                bsr     get_base        ;TOS-Basisadresse und
                bne     error25
                bsr     get_length      ;TOS-Laenge holen
                bne     error25
                lsr.l   #4,D1           ;Laenge/16, da 4*move.l
                lea     buffer,A0       ;Puffer fuer das TOS
mkplis18:       move.l  (A1)+,(A0)+     ;TOS kopieren
                move.l  (A1)+,(A0)+
                move.l  (A1)+,(A0)+
                move.l  (A1)+,(A0)+
                subq.l  #1,D1
                bne.s   mkplis18
                bra     mkplis2

****************************************
* TOS von Disk lesen

mkplis19:       lea     PCRD(read_tos),A0
                bsr     print_line

                movea.l A6,A0           ;Zeiger auf Filename
mkplis20:       cmpi.b  #' ',(A6)+      ;Filename-Ende suchen
                bhi.s   mkplis20
                clr.b   -1(A6)          ;und mit Nullbyte abschliessen

                lea     buffer,A1       ;Puffer fuer das TOS

                bsr     read_length
                cmp.l   #max_length,D0  ;Zu lang?
                bhi     error5          ;Ja! =>
                bsr     read_file       ;TOS-File einlesen

                move.l  D0,D7
                bsr     get_length
                cmp.l   D1,D7
                beq     mkplis2
                bhi.s   mkpli20a        ;TOS laenger als angegeben? Ja! =>
                bsr     error4          ;TOS kuerzer als angegeben
                bra     mkplis2

mkpli20a:       bsr     error3
                bra     mkplis2


****************************************
* Ausgabeformat festlegen

mkplis21:       addq.l  #1,A6
                bsr     get_term        ;Anzahl der Dateien holen
                move.l  D1,output_format ;und sichern
mkplis22:       bsr     skip_space      ;Ist Relozieradresse angegeben?
                bne.s   mkpli22a        ;Nein! =>

                bsr     get_term        ;Relozieradresse holen
                move.l  D1,D0
                bra.s   mkpli22b

mkpli22a:       moveq   #0,D0           ;Relozieradresse auf 0 setzen

mkpli22b:       move.l  #$52454c4f,var_name ;Relozieradresse schreiben
                move.l  #$43414452,var_name+4 /* 'RELOCADR' */
                bsr     write_var
                bra     mkplis2


****************************************
* Aktuellen Pfad aendern

mkplis23:       addq.l  #1,A6

                lea     PCRD(path_text),A0
                bsr     print_line

                movea.l A6,A0           ;Zeiger auf Pfad
mkplis24:       cmpi.b  #' ',(A6)+      ;Pfad-Ende suchen
                bhi.s   mkplis24
                clr.b   -1(A6)          ;und mit Nullbyte abschliessen

                bsr     print_line

                move.l  A0,-(SP)
                move.w  #$003B,-(SP)    ;Dsetpath
                trap    #1              ;GEMDOS 59
                addq.l  #6,SP

                lea     PCRD(path_end_text),A0
                bsr     print_line
                bra     mkplis2


****************************************
* Batchdatei-Ende

mkplis25:       tst.w   if_count        ;noch eine offener _IF-Befehl?
                bne     error20         ;Ja! =>
                tst.w   filenr          ;Ende des 2. Batchfile?
                bne.s   mkpli25a        ;Ja! =>
                bsr     get_base        ;Alle Systemvariable auslesen
                move.l  A1,base
                bsr     get_length
                move.l  D1,length
                bsr     get_reloc_adr
                move.l  A1,reloc_adr
                bsr     get_width
                move.l  D1,width

                move.l  #$43524300,var_name /* 'CRC' */
                clr.l   var_name+4
                bsr     read_var
                move.w  D1,checksum

                rts

****************************************
* Ende eines 2. Batchfiles

mkpli25a:       subq.w  #1,filenr       ;Flag fuer 2. Batchfile loeschen
                move.w  (SP)+,if_count  ;Bedingungszaehler,
                move.l  (SP)+,line      ;Zeilennummer und
                movea.l (SP)+,A6        ;Textpointer wieder herstellen
                bra     mkplis2


****************************************
* CRC-Berechnung aktivieren

mkplis26:       moveq   #1,D0           ;Eine 1 in Variable 'CRC' schreiben
                move.l  #$43524300,var_name /* 'CRC' */
                clr.l   var_name+4
                bsr     write_var
                bra     mkplis2


****************************************
* Focierte Relozierausnahmen auswerten

mkplis27:       addq.l  #1,A6
                lea     PCRB(forced_table),A0 ;Ausnahmentabellenende suchen
mkplis28:       tst.l   (A0)+
                bne.s   mkplis28
                subq.l  #4,A0
mkplis29:       bsr     skip_space
                bne     error0
                bsr     get_term        ;Adresse holen
                move.l  D1,(A0)+        ;und in Tabelle speichern
                bsr     skip_space
                bne     mkplis2
                cmpi.b  #',',(A6)+      ;noch ein Wert?
                beq.s   mkplis29        ;Ja! =>
                bra     error2


****************************************
* Einfache Variable (#) setzen

mkplis30:       addq.l  #1,A6
                bsr     get_term        ;Wert holen
                move.l  D1,variable     ;und in Variable speichern
                bra     mkplis2


****************************************
* 2. Batchfile laden

mkplis31:       addq.l  #1,A6

                tst.w   filenr
                bne     error19

                lea     PCRD(read_text),A0
                bsr     print_line

                movea.l A6,A0           ;Zeiger auf Filename
mkplis32:       cmpi.b  #' ',(A6)+      ;Filename-Ende suchen
                bhi.s   mkplis32
                clr.b   -1(A6)          ;und mit Nullbyte abschliessen

                lea     btchbuf2,A1

                bsr     read_length     ;Laenge holen
                cmp.l   #$00008000,D0   ;Zu lang?
                bhs     error6          ;Ja! =>
                bsr     read_file       ;Batch-File einlesen

                move.l  A6,-(SP)        ;Textpointer,
                move.l  PCRB(line),-(SP)  ;Zeilennummer und
                move.w  PCRB(if_count),-(SP) ;Bedingungszaehler sichern

                addq.w  #1,filenr       ;Flag fuer 2.Batchdatei setzen

                lea     btchbuf2,A6    ;2. Batchfile
                clr.b   0(A6,D0.l)      ;mit Nullbyte abschliessen

                bra     mkplis0         ;Auswertung starten


****************************************
* Auswertung aller Zeilen, die mit . anfangen

mkplis33:       movea.l A6,A0
                move.b  1(A6),D0
                cmp.b   #'0',D0         ;dezimaler Patchoffset?
                blo.s   mkplis34        ;Nein! =>
                cmp.b   #'9',D0
                ble     mkplis3         ;Ja! =>
mkplis34:       bsr     get_vname
                bsr     skip_space
                bne     error0
                addq.l  #1,A6
                cmp.b   #':',D1         ;~ setzen?
                beq.s   mkplis35        ;Ja! =>
                cmp.b   #'=',D1         ;Variable setzen?
                bne.s   mkpli35a        ;Nein! => Variable ist Patchoffset
                bsr     skip_space
                bne     error0
                bsr     get_term        ;Wert holen
                move.l  D1,D0
                exg     A0,A6
                bsr     get_vname       ;Variablenname erneut holen
                movea.l A0,A6
                bsr     write_var       ;und Variable schreiben
                bra     mkplis2

****************************************
* ~ auf .VARIABLE setzen

mkplis35:       move.l  PCRB(current_adr),D0
                bsr     write_var
                bra     mkplis1

****************************************
* .VARIABLE ist Patchoffset

mkpli35a:       movea.l A0,A6           ;alten Textpointer zurueckholen
                bra     mkplis3     ;Patch auswerten


****************************************
* PRINT-Befehl (/)

mkplis36:       addq.l  #1,A6
mkplis37:       clr.w   on_off          ;Flag loeschen
                bsr     skip_space
                bne     error2
                cmpi.b  #"'",D1         ;Textausgabe?
                beq.s   mkplis38        ;Ja! =>
                cmp.b   #'"',D1
                beq.s   mkplis38        ;Ja! =>
                cmpi.b  #'$',D1         ;Hexadezimale Ausgabe
                bne.s   mkpli37a        ;Nein! =>
                move.b  (A6)+,on_off    ;Flag setzen
mkpli37a:       bsr     get_term        ;Wert holen
                move.l  D1,D0
                tst.w   on_off          ;Hexadezimal?
                beq.s   mkpli37b        ;Nein! =>
                bsr     hex_out         ;hexadezimal ausgeben
                bra.s   mkpli37c
mkpli37b:       bsr     dez_out         ;dezimal ausgeben
mkpli37c:       bsr     skip_space      ;Zeilenende?
                bne.s   mkplis40        ;Ja! =>
                cmpi.b  #',',(A6)+      ;noch ein Wert?
                beq.s   mkplis37        ;Ja! =>
                bra     error2
mkplis38:       addq.l  #1,A6
                movea.l A6,A0           ;Zeiger auf Textanfang setzen
mkplis39:       move.b  (A6)+,D0
                beq     error2
                cmp.b   #$0D,D0
                beq     error2
                cmp.b   D1,D0           ;Textende?
                bne.s   mkplis39        ;Nein! => weiter suchen
                clr.b   -1(A6)          ;Textende mit Nullbyte abschliessen
                bsr     print_line      ;Text ausgeben
                bra.s   mkpli37c
mkplis40:       lea     PCRD(lf_text),A0  ;Ausgabe mit LF/CR
                bsr     print_line      ;abschliessen
                bra     mkplis2


****************************************
* Befehle, die mit _ anfangen, ausfuehren

mkplis41:       addq.l  #1,A6
                lea     PCRD(com_tab),A1  ;Befehlstabelle holen
                move.w  (A1)+,D1        ;Anzahl der Befehle holen
mkplis42:       moveq   #7,D0           ;Ein Befehl ist 8 Bytes lang
                movea.l A6,A0
mkplis43:       move.b  (A0)+,D2        ;Zeichen holen
                beq     error0          ;Textende? Ja! =>
                cmp.b   #$0D,D2         ;Zeilenende?
                beq     error0          ;Ja! =>
                and.b   #$DF,D2         ;auf Grossschrift wandeln
                cmp.b   (A1)+,D2        ;mit Befehlstabelle vergleichen
                bne.s   mkplis45        ;Gleich? Nein! =>
                tst.b   (A1)            ;Befehl komplett verglichen?
                beq.s   mkplis44        ;Ja! =>
                subq.l  #1,D0           ;Alle 8 Zeichen verglichen?
                bpl.s   mkplis43        ;Nein! =>
                movea.l A0,A6           ;Textpointer aktualisieren
                movea.l (A1),A1         ;Befehlsadresse holen
                jmp     (A1)            ;und ausfuehren
mkplis44:       movea.l A0,A6           ;Textpointer aktualisieren
                adda.l  D0,A1           ;Pointer auf Befehlsadresse setzen
                movea.l (A1),A1         ;diese holen
                jmp     (A1)            ;und Befehl ausfuehren
mkplis45:       adda.l  D0,A1           ;Pointer auf
                addq.l  #4,A1           ;naechsten Eintrag setzen
                subq.w  #1,D1           ;Alle Befehle verglichen?
                bne.s   mkplis42        ;Nein! =>
                bra     error15

****************************************
* _IF-Befehl

mkplis46:       bsr     get_term        ;Ergebnis des Vergleichs holen
                tst.l   D1              ;ist es FALSE (0)?
                beq.s   mkplis47        ;Ja! =>
                subq.l  #1,D1           ;ist es TRUE (1)?
                bne     error16         ;Nein! =>
                addq.w  #1,if_count     ;IF-Verschachtelung um 1 erhoehen
                move.w  #$8001,-(SP)    ;Wert fuer IF/TRUE auf den Stack legen
                bra     mkplis2

mkplis47:       moveq   #0,D7           ;Anzahl der IF-Befehle initialisieren

mkplis48:       move.b  (A6)+,D0        ;naechsten Zeilenanfang suchen
                beq     mkplis25
                cmp.b   #$0A,D0
                bne.s   mkplis48
                addq.l  #1,line

                bsr     skip_space
                bne.s   mkplis48

                cmpi.b  #'_',(A6)+      ;Faengt Zeile mit _ an?
                bne.s   mkplis48        ;Nein! =>

                lea     PCRD(if_txt),A0   ;nach _IF-Befehl suchen
                movea.l A6,A1
mkpli48a:       move.b  (A6)+,D0        ;Zeichen holen
                and.b   #$DF,D0         ;auf Grossschrift wandeln
                cmp.b   (A0)+,D0        ;Ist es gleich?
                bne.s   mkpli48b        ;Nein! =>
                tst.b   (A0)            ;Befehl komplett verglichen?
                bne.s   mkpli48a        ;Nein! =>
                addq.l  #1,D7           ;Anzahl der IF-Befehle incrementieren
                bra.s   mkplis48        ;naechste Zeile

mkpli48b:       lea     PCRD(else_txt),A0 ;nach _ELSE-Befehl suchen
                movea.l A1,A6
mkplis49:       move.b  (A6)+,D0        ;Zeichen holen
                and.b   #$DF,D0         ;auf Grossschrift wandeln
                cmp.b   (A0)+,D0        ;Ist es gleich?
                bne.s   mkpli49a        ;Nein! =>
                tst.b   (A0)            ;Befehl komplett verglichen?
                bne.s   mkplis49        ;Nein! =>
                tst.l   D7              ;noch offene IF-Befehle?
                bne.s   mkplis48        ;Ja! =>
                addq.w  #1,if_count     ;IF-Verschachtelung um 1 erhoehen
                move.w  #$8000,-(SP)    ;Wert fuer ELSE/FALSE auf den Stack legen
                bra     mkplis2

mkpli49a:       lea     PCRD(endif_txt),A0 ;nach _ENDIF-Befehl suchen
                movea.l A1,A6
mkpli49b:       move.b  (A6)+,D0        ;Zeichen holen
                and.b   #$DF,D0         ;auf Grossschrift wandeln
                cmp.b   (A0)+,D0        ;Ist es gleich?
                bne.s   mkplis48        ;Nein! =>
                tst.b   (A0)            ;Befehl komplett verglichen?
                bne.s   mkpli49b        ;Nein! =>
                tst.l   D7              ;noch offene IF-Befehle?
                beq     mkplis2         ;Nein! =>
                subq.l  #1,D7           ;Anzahl der IF-Befehle dekrementieren
                bra.s   mkplis48

****************************************
* _ELSE-Befehl

mkplis50:       tst.w   if_count        ;Ist ein offener IF-Befehl vorhanden?
                beq     error17         ;Nein! =>
                subq.w  #1,if_count     ;Anzahl der IF-Befehle dekrementieren
                cmpi.w  #$8001,(SP)+    ;War es ein IF/TRUE?
                bne     error17         ;Nein! =>

                moveq   #0,D7           ;Anzahl der IF-Befehle initialisieren

mkplis51:       move.b  (A6)+,D0        ;naechsten Zeilenanfang suchen
                beq     mkplis25
                cmp.b   #$0A,D0
                bne.s   mkplis51
                addq.l  #1,line

                bsr     skip_space
                bne.s   mkplis51

                cmpi.b  #'_',(A6)+      ;Faengt Zeile mit _ an?
                bne.s   mkplis51        ;Nein! =>

                lea     PCRD(if_txt),A0   ;nach _IF-Befehl suchen
                movea.l A6,A1
mkpli51a:       move.b  (A6)+,D0        ;Zeichen holen
                and.b   #$DF,D0         ;auf Grossschrift wandeln
                cmp.b   (A0)+,D0        ;Ist es gleich?
                bne.s   mkplis52        ;Nein! =>
                tst.b   (A0)            ;Befehl komplett verglichen?
                bne.s   mkpli51a        ;Nein! =>
                addq.l  #1,D7           ;Anzahl der IF-Befehle incrementieren
                bra.s   mkplis51        ;naechste Zeile

mkplis52:       lea     PCRD(endif_txt),A0 ;nach _ENDIF-Befehl suchen
                movea.l A1,A6
mkpli52a:       move.b  (A6)+,D0        ;Zeichen holen
                and.b   #$DF,D0         ;auf Grossschrift wandeln
                cmp.b   (A0)+,D0        ;Ist es gleich?
                bne.s   mkplis51        ;Nein! =>
                tst.b   (A0)            ;Befehl komplett verglichen?
                bne.s   mkpli52a        ;Nein! =>
                tst.l   D7              ;noch offene IF-Befehle?
                beq     mkplis2         ;Nein! =>
                subq.l  #1,D7           ;Anzahl der IF-Befehle dekrementieren
                bra.s   mkplis51

****************************************
* _ENDIF-Befehl

mkplis53:       tst.w   if_count        ;Ist ein offenener IF-Befehl vorhanden?
                beq     error18         ;Nein! =>
                subq.w  #1,if_count     ;Anzahl der IF-Befehle dekrementieren
                tst.w   (SP)+           ;Wert fuer IF-Befehl auf dem Stack?
                bpl     error18         ;Nein! =>
                bra     mkplis2

****************************************
* ERROR-Befehl

mkplis54:       bsr     skip_space
                bne     error22         ;'_ERROR'-Message ohne zus. Text ausgeben
                cmpi.b  #"'",(A6)+      ;ein ' als Textanfang?
                beq.s   mkpli54a        ;Ja! =>
                cmpi.b  #'"',D1         ;ein " als Textanfang?
                bne     error2          ;Nein! =>

mkpli54a:       lea     PCRD(error_txt),A0 ;'Fehler: '
                bsr     print_line      ;ausgeben

                movea.l A6,A0           ;Zeiger auf Textanfang setzen
mkplis55:       move.b  (A6)+,D0        ;Zeichen holen
                beq     error0          ;Batchdatei zu Ende? Ja! =>
                cmp.b   #$0D,D0         ;Zeile zu Ende?
                beq     error0          ;Ja! =>
                cmp.b   D1,D0           ;Textende?
                bne.s   mkplis55        ;Nein! => weiter suchen
                clr.b   -(A6)           ;Textende mit Nullbyte abschliessen
                moveq   #0,D0           ;Fehlermeldung
                bra     user_error      ;ausgeben

****************************************
* _EVEN-Befehl

mkplis56:       move.l  PCRB(current_adr),D0 ;akt. Adresse holen
                btst    #0,D0           ;gerade?
                beq     mkplis2         ;Ja! => fertig
                moveq   #1,D1           ;1 Byte soll geschrieben werden
                add.l   D1,current_adr  ;akt. Adresse aktualisieren
                move.l  D1,(A3)+        ;1 Byte (=0 s.u.)
                move.l  D0,(A3)+        ;an akt. Adresse schreiben
                clr.w   (A3)+           ;das Null-Byte + EVEN der patch_list
                bra     mkplis2         ;zurueck


****************************************
* Routinen zum auslesen der Systemvariablen

get_base:       move.l  #'BASE',var_name ;Variablenname setzen
                clr.l   var_name+4
                bsr     read_var        ;Variable auslesen
                bne.s   getbas1         ;Nicht gefunden? Ja! =>
                movea.l D1,A1           ;Variablenwert nach A1
                moveq   #0,D1           ;'OK' setzen
                rts
getbas1:        movea.l #def_base,A1    ;Defaultwert nach A1
                moveq   #-1,D1          ;'Fehler' setzen
                rts

get_length:     move.l  #$4c454e47,var_name ;Variablenname setzen
                move.l  #$54480000,var_name+4 /* 'LENGTH' */
                bsr     read_var        ;Variable auslesen
                bne.s   getlen1         ;Nicht gefunden? Ja! ?>
                cmp.l   #max_length,D1  ;Variablenwert groesser als TOS-Buffer?
                bhi     error24         ;Ja! =>
                moveq   #0,D0           ;'OK' setzen
                rts
getlen1:        move.l  #def_length,D1  ;Defaultwert setzen
                moveq   #-1,D0          ;'Fehler' setzen
                rts

get_reloc_adr:  move.l  #$52454c4f,var_name ;Variablenname setzen
                move.l  #$43414452,var_name+4 /* 'RELOCADR' */
                bsr     read_var        ;Variablenwert lesen
                bne.s   getrel1         ;Nicht vorhanden? Ja! =>
                movea.l D1,A1           ;Variablenwert nach A1
                moveq   #0,D0           ;'OK' setzen
                rts
getrel1:        suba.l  A1,A1           ;Defaultwert (= nicht Relozieren) setzen
                moveq   #-1,D0          ;'Fehler' setzen
                rts

get_width:      move.l  #$57494454,var_name ;Variablenname setzen
                move.l  #$48000000,var_name+4 /* 'WIDTH' */
                bsr     read_var        ;Variablewert lesen
                bne.s   getwid2         ;Nicht vorhanden? Ja! =>
                cmp.l   #8,D1           ;Wert OK?
                beq.s   getwid1         ;Ja! =>
                cmp.l   #16,D1          ;Wert OK?
                beq.s   getwid1         ;Ja! =>
                cmp.l   #32,D1          ;Wert OK?
                beq.s   getwid1         ;Ja! =>
                cmp.l   #64,D1          ;Wert OK?
                /* bne     PCRD(error23_t)       ;Nein! => ; BUG */
                dc.w    $6600
                dc.w    $114d           ; (__etext+(error23_t-__data))-.
getwid1:        moveq   #0,D0           ;'OK' setzen
                rts
getwid2:        moveq   #def_width,D1   ;Defaultwert setzen
                moveq   #-1,D0          ;'Fehler' setzen
                rts


****************************************
* get_term: Ausdruck aus Text auswerten
* IN  A6.l: Pointer auf Ausdruck
* OUT D0.l: Wert des Ausdrucks
*     D1.w: Prioritaet (high) und Abbruchzeichen (low)

get_term:       movem.l D2-A1,-(SP)     ;Benutzte Register sichern
                moveq   #0,D7           ;Anzahl der Verschachtelungen resetten
                bsr     get_number      ;1. Zahl holen
                move.l  D1,-(SP)        ;und sichern
                bsr     get_command     ;1. Kommando holen
                move.w  D1,-(SP)        ;und sichern
                and.w   #$FF00,D1       ;Fertig?
                beq     gettere2        ;Ja! =>
getter1:        addq.w  #1,D7           ;Anzahl der Verschachtelungen erhoehen
getter2:        bsr     get_number      ;naechste Zahl holen
                move.l  D1,-(SP)        ;und sichern
                bsr     get_command     ;naechstes Kommando holen
                move.w  D1,-(SP)        ;und sichern
getter3:        move.b  6(SP),D0        ;Ist Prioritaet des 1. Kommandos kleiner
                cmp.b   (SP),D0         ;als Prioritaet des 2. Kommandos?
                blo.s   getter1         ;Ja! =>
                move.w  (SP)+,D3        ;Werte fuer
                move.l  (SP)+,D2        ;einen Rechenschritt
                move.w  (SP)+,D0        ;vom Stack
                move.l  (SP)+,D1        ;holen
                subq.b  #1,D0           ;entsprechende Rechenroutine aufrufen
                beq.s   lower
                subq.b  #1,D0
                beq.s   lower_equal
                subq.b  #1,D0
                beq.s   unequal
                subq.b  #1,D0
                beq.s   greater
                subq.b  #1,D0
                beq.s   greater_equal
                subq.b  #1,D0
                beq.s   equal
                subq.b  #1,D0
                beq.s   do_xor
                subq.b  #1,D0
                beq.s   do_or
                subq.b  #1,D0
                beq.s   do_and
                subq.b  #1,D0
                beq.s   do_add
                subq.b  #1,D0
                beq.s   do_sub
                subq.b  #1,D0
                beq.s   do_mult
                subq.b  #1,D0
                beq     do_div
                subq.b  #1,D0
                beq     do_pot
                bra     error8          ;Rechenroutine nicht existend! => Fehler

lower:          cmp.l   D2,D1           ;Kleiner-Vergleich
                blo.s   end_true
                bra.s   end_false

lower_equal:    cmp.l   D2,D1           ;Kleinergleich-Vergleich
                bls.s   end_true
                bra.s   end_false

unequal:        cmp.l   D2,D1           ;Ungleichheits-Vergleich
                bne.s   end_true
                bra.s   end_false

greater:        cmp.l   D2,D1           ;Groesser-Vergleich
                bhi.s   end_true
                bra.s   end_false

greater_equal:  cmp.l   D2,D1           ;Groessergleich-Vergleich
                bhs.s   end_true
                bra.s   end_false

equal:          cmp.l   D2,D1           ;Gleichheits-Vergleich
                beq.s   end_true


end_false:      moveq   #0,D1           ;Ende fuer Bedingung nicht erfuellt
                bra     end_do

end_true:       moveq   #1,D1           ;Ende fuer Bedingung erfuellt
                bra     end_do



do_xor:         eor.l   D2,D1           ;Exklusivoder-Routine
                bra     end_do

do_or:          or.l    D2,D1           ;Oder-Routine
                bra     end_do

do_and:         and.l   D2,D1           ;Und-Routine
                bra     end_do

do_add:         add.l   D2,D1           ;Additions-Routine
                bra     end_do

do_sub:         sub.l   D2,D1           ;Subtraktions-Routine
                bra     end_do

do_mult:        cmp.l   D2,D1           ;Multiplikations-Routine
                bhi.s   do_mult1        ;den groesseren Wert
                exg     D1,D2           ;nach D1 vertauschen
do_mult1:       move.l  D2,D0           ;Ist kleinerer Wert
                swap    D0              ;groesser als
                tst.w   D0              ;$10000?
                bne     error7          ;Ja! => ueberlauf
                moveq   #0,D0           ;Lowword von D1
                move.w  D1,D0           ;mit D2
                mulu    D2,D0           ;multiplizieren
                clr.w   D1              ;Highword von D1
                swap    D1              ;mit D2
                mulu    D2,D1           ;multiplizieren
                swap    D1              ;Hat ueberlauf
                tst.w   D1              ;stattgefunden?
                bne     error7          ;Ja! =>
                add.l   D0,D1           ;Summe beider Produkte = Ergebnis
                bcs     error7          ;ueberlauf? Ja! =>
                bra.s   end_do

do_div:         tst.l   D2              ;Divisions-Routine
                beq     error9          ;Divisor = 0? Ja! => Division durch 0
                move.l  D2,D0           ;Divisor
                swap    D0              ;groesser als
                tst.w   D0              ;$10000?
                bne.s   do_div2         ;Ja! =>
                swap    D1              ;Highword des Dividenten (D1)
                moveq   #0,D0           ;durch
                move.w  D1,D0           ;Divisor (D2)
                divu    D2,D0           ;teilen
                moveq   #0,D4           ;Quotient
                move.w  D0,D4           ;mal $10000
                swap    D4              ;nach D4
                swap    D1              ;Lowword des Dividenten (D1)
                move.w  D1,D0           ;durch Divisor (D2)
                divu    D2,D0           ;teilen
                move.w  D0,D4           ;Quotient zu D4 'addieren'
                move.l  D4,D1           ;Ergebnis nach D1
                bra.s   end_do

do_div2:        moveq   #0,D0           ;Zaehler fuer das Ergebnis auf 0 setzen
                bra.s   do_div4
do_div3:        addq.l  #1,D0           ;Ergebnis erhoehen
                sub.l   D2,D1           ;Divisor 1* vom Dividenten abziehen
do_div4:        cmp.l   D2,D1           ;Divident noch groesser als Divisor
                bhs.s   do_div3         ;Ja! =>
                move.l  D0,D1           ;Ergenis nach D1
                bra.s   end_do

do_pot:         tst.l   D2              ;Routine fuer Potenzen
                bne.s   do_pot1         ;Exponent = 0? Nein! =>
                moveq   #1,D1           ;Ergebnis = 1
                bra.s   end_do
do_pot1:        cmp.l   #1,D2           ;Exponent = 1
                beq.s   end_do          ;Ja! => Fertig

                moveq   #1,D4           ;Ergebnis auf 1 setzen
                move.l  D1,D0           ;Highword
                swap    D0              ;der Basis (D1)
                tst.w   D0              ;<> 0?
                bne     error7          ;Ja! => ueberlauf
do_pot3:        moveq   #0,D0           ;Lowword
                move.w  D4,D0           ;des Ergebnisses (D4)
                mulu    D1,D0           ;mit Basis (D1) multiplizieren
                clr.w   D4              ;Highword
                swap    D4              ;des Ergebnisses (D4)
                mulu    D1,D4           ;mit Basis (D1) multiplizieren
                swap    D4              ;mal $10000
                tst.w   D4              ;ueberlauf bei Multiplikation mit Basis?
                bne     error7          ;Ja! =>
                add.l   D0,D4           ;Summe der Produkte = neues Ergebnis
                subq.l  #1,D2           ;Exponent erniedrigen
                bne.s   do_pot3         ;Exponent schon 0? Nein! => Weiter
                move.l  D4,D1           ;Ergebnis nach D1


end_do:         move.l  D1,-(SP)        ;Ergebnis und
                move.w  D3,-(SP)        ;2. Kommando wieder auf den Stack
                and.w   #$FF00,D3       ;Fertig?
                bne     getter2         ;Nein! =>

gettere:        subq.w  #1,D7           ;Eine Verschachtelung weniger
                bne     getter3         ;Fertig? Nein! =>
gettere2:       move.w  (SP)+,D0        ;Ergebnis und
                move.l  (SP)+,D1        ;Prioritaet/Abbruchzeichen holen
                movem.l (SP)+,D2-A1     ;Register wiederholen
                rts

****************************************
* get_number: Holt Zahl aus Text
* IN  A6.l: Pointer auf Zahl
* OUT D0.l: Fehlerflag
*     D1.l: Zahl

get_number:     bsr     skip_space      ;Zeilenende?
                bne     error0          ;Ja! =>
                cmpi.b  #'(',(A6)       ;Klammerausdruck?
                bne.s   getnum1         ;Nein! =>
                addq.l  #1,A6           ;Klammer ueberspringen
                bsr     get_term        ;Ausdruck holen
                cmpi.b  #')',D0         ;Ist abschliessende Klammer vorhanden?
                bne     error10         ;Nein! =>
                rts
getnum1:        bsr     get_signed      ;Vorzeichenbehaftete Zahl holen
                bne     error11         ;Fehler? Ja! =>
                rts

****************************************
* get_command: Holt Kommando und dessen Prioriaet aus Text
* IN  A6.l: Pointer auf Kommando
* OUT D1.w: Prioritaet (High) und Abbruchzeichen (Low)

get_command:    bsr     skip_space      ;Zeilenende?
                bne.s   getcmd5         ;Ja! =>

getcmd1:        lea     PCRD(priority),A0 ;Tabellenadresse holen
                move.w  (A0)+,D0        ;Anzahl der Eintraege holen

getcmd2:        movea.l A6,A1           ;Textpointer nach A1
                moveq   #8-1,D1         ;Kommandoname ist max. 8 Stellen
getcmd3:        move.b  (A1)+,D3        ;Zeichen holen
                cmp.b   #'a',D3         ;Kleinschrift?
                blo.s   getcmd3a        ;Nein! =>
                cmp.b   #'z',D3
                bhi.s   getcmd3a        ;Nein! =>
                and.b   #$DF,D3         ;Ja! : In Grossschrift wandeln
getcmd3a:       cmp.b   (A0)+,D3        ;Mit Tabelle vergleichen
                bne.s   getcmd4         ;Ungleich? Ja! =>
                cmpi.b  #' ',(A0)       ;Kommando gefunden?
                beq.s   getcmd6         ;Ja! =>
                subq.l  #1,D1           ;Schon alle 8 Zeichen verglichen?
                bpl.s   getcmd3         ;Nein! =>
                bra.s   getcmd7

getcmd4:        adda.l  D1,A0           ;Naechsten Tabelleneintrag
                addq.l  #2,A0           ;berechnen
                subq.w  #1,D0           ;Schon die ganze Tabelle verglichen?
                bne.s   getcmd2         ;Nein! =>

getcmd5:        moveq   #0,D1           ;0 fuer Abbruch/kein Kommando
                rts

getcmd6:        adda.l  D1,A0           ;Zeiger auf Prioritaet setzen
getcmd7:        move.w  (A0),D1         ;Prioritaet holen
                movea.l A1,A6           ;Textpointer aktualisieren
                rts

****************************************
* get_extension: Holt die Erweiterung eines Ausdrucks
* IN  A6.l: Pointer auf Extension
* OUT D2.l: Extension (B/W/L)

get_extension:  movem.l D0-D1,-(SP)     ;Benutzte Register sichern
                bsr     skip_space      ;Zeilenende?
                bne.s   getext1         ;Ja! =>
                cmpi.b  #'.',(A6)       ;Extension vorhanden?
                bne.s   getext1         ;Nein! =>
getext0:        addq.l  #1,A6           ;'.' ueberspringen
                move.b  (A6)+,D0        ;Wert der Extension holen
                and.b   #$DF,D0         ;Auf Grossbuchstaben wandeln
                cmp.b   #'B',D0         ;Ist es Byte?
                beq.s   getext1         ;Ja! =>
                moveq   #2,D2           ;Word schon mal setzen
                cmp.b   #'W',D0         ;Ist es Word?
                beq.s   getext2         ;Ja! =>
                moveq   #4,D2           ;Long schon mal setzen
                cmp.b   #'L',D0         ;Ist es Long?
                beq.s   getext2         ;Nein! =>
                subq.l  #2,A6           ;Textpointer zuruecksetzen
getext1:        moveq   #1,D2           ;Default-Extension (B) setzen
getext2:        movem.l (SP)+,D0-D1
                rts

****************************************
* get_signed: Vorzeichenbehaftete Zahl aus Text holen
* IN  A6.l: Pointer auf Zahl
* OUT D1.l: Zahl

get_signed:     bsr     skip_space      ;Zeilenende?
                bne     error0          ;Ja! =>
                cmpi.b  #'-',(A6)       ;Negatives Vorzeichen?
                bne.s   get_unsigned    ;Nein! => Normale Zahl holen

                addq.l  #1,A6           ;Vorzeichen ueberspringen
                bsr.s   get_unsigned    ;Zahl holen
                neg.l   D1              ;und negieren
                tst.l   D2              ;Fehlerflag setzen
                rts

****************************************
* get_unsigned: Zahl aus Text holen
* IN  A6.l: Pointer auf Zahl
* OUT D0.l: 1. Zeichen nach der Zahl
*     D1.l: Zahl
*     D2.l: Fehlerflag

get_unsigned:   bsr     skip_space      ;Zeilenende?
                bne.s   get_error       ;Ja! =>
                cmpi.b  #'~',(A6)       ;Ist es die Tilde?
                beq.s   get_current     ;Ja! =>
                cmpi.b  #'.',(A6)       ;Ist es Dezimalzahl oder Variable?
                bne.s   get_hexbyte     ;Nein! =>
                cmpi.b  #'0',1(A6)      ;Ist es eine Dezimalzahl?
                blo.s   get_var         ;Nein! =>
                cmpi.b  #'9',1(A6)
                bls.s   get_dezbyte     ;Ja! =>

get_var:        bsr     get_vname       ;Variablenname holen
                bsr     read_var        ;Wert der Variable holen
                bne     error13         ;Variable nicht gefunden? Ja! =>
                moveq   #0,D2           ;'OK' setzen
                rts

get_current:    addq.l  #1,A6           ;'#' uebespringen
                move.l  PCRB(current_adr),D1 ;Aktuelle Adresse holen
                moveq   #0,D2           ;'OK' setzen
                rts

get_error:      moveq   #0,D1           ;Zahl auf 0 setzen
                moveq   #-1,D2          ;'Fehler' setzen
                rts

****************************************
* get_hexbyte: Hexadezimalzahl aus Text holen
* IN  A6.l: Pointer auf Zahl
* OUT D0.l: 1. Zeichen nach der Zahl
*     D1.l: Zahl
*     D2.l: Fehlerflag

get_hexbyte:    moveq   #-1,D2          ;'Fehler' setzen
                moveq   #0,D1           ;Zahl auf 0 setzen
gethexb1:       moveq   #0,D0           ;Zeichen
                move.b  (A6)+,D0        ;holen
                eori.b  #'0',D0         ;Ist eine
                cmp.b   #9,D0           ;Hex-Zahl?
                bls.s   gethexb2    ;Ja! =>
                eori.b  #'0',D0
                and.b   #$DF,D0         ;in Grossschrift wandeln
                sub.b   #'A',D0         ;Ist es ein Hex-Zeichen?
                bmi.s   gethexb3    ;Nein! =>
                cmp.b   #6,D0
                bhs.s   gethexb3    ;Nein! =>
                add.b   #10,D0
gethexb2:       lsl.l   #4,D1           ;Zahl *$10
                or.b    D0,D1           ;Ziffer addieren
                moveq   #0,D2           ;'OK' setzen
                bra.s   gethexb1    ;Weiter
gethexb3:       move.b  -(A6),D0        ;Pointer korrigieren
                tst.l   D2
                rts

****************************************
* get_dezbyte: Dezimalzahl aus Text holen

get_dezbyte:    moveq   #-1,D2
                moveq   #0,D1

                cmpi.b  #".",(A6)+
                bne.s   getdezb3

getdezb1:       moveq   #0,D0
                move.b  (A6)+,D0
                sub.b   #'0',D0
                bmi.s   getdezb3
                cmp.b   #9,D0
                bhi.s   getdezb3

                moveq   #0,D2
                add.l   D1,D1
                bcs.s   getdezb2
                add.l   D1,D0
                bcs.s   getdezb2
                lsl.l   #2,D1
                bcs.s   getdezb2
                add.l   D0,D1
                bcc.s   getdezb1
getdezb2:       moveq   #-1,D2
getdezb3:       move.b  -(A6),D0
                tst.l   D2
                rts

get_vname:      cmpi.b  #'.',(A6)+
                bne.s   getvn6
                movem.l D0/A0,-(SP)
                moveq   #0,D1
                lea     PCRB(var_name),A0
                clr.l   (A0)
                clr.l   4(A0)
getvn1:         move.b  (A6)+,D0
                tst.l   D1
                beq.s   getvn2
                cmp.b   #'0',D0
                blo.s   getvn5
                cmp.b   #'9',D0
                bls.s   getvn3
getvn2:         cmp.b   #'_',D0
                beq.s   getvn3
                and.b   #$DF,D0
                cmp.b   #'A',D0
                blo.s   getvn5
                cmp.b   #'Z',D0
                bhi.s   getvn5
getvn3:         addq.l  #1,D1
                cmp.l   #8,D1
                bhi.s   getvn1
                move.b  D0,(A0)+
                bra.s   getvn1
getvn5:         movem.l (SP)+,D0/A0
                subq.l  #1,A6
                tst.b   D1
                bne.s   getvn7
getvn6:         subq.l  #1,A6
                moveq   #-1,D1
                rts
getvn7:         moveq   #0,D1
                rts

search_var:     move.l  D0,-(SP)
                lea     PCRB(varname),A1
srchvar1:       tst.b   (A1)
                beq.s   srchvar4
                moveq   #7,D0
                lea     PCRB(var_name),A0
srchvar2:       cmpm.b  (A0)+,(A1)+
                bne.s   srchvar3
                subq.l  #1,D0
                bpl.s   srchvar2
                move.l  (SP)+,D0
                subq.l  #8,A1
                move.l  A1,D1
                sub.l   #varname,D1
                lsr.l   #1,D1
                lea     PCRB(varvalue),A0
                adda.l  D1,A0
                moveq   #0,D1
                rts
srchvar3:       adda.l  D0,A1
                bra.s   srchvar1
srchvar4:       move.l  (SP)+,D0
                move.l  A1,D1
                sub.l   #varname,D1
                lsr.l   #1,D1
                lea     PCRB(varvalue),A0
                adda.l  D1,A0
                moveq   #-1,D1
                rts

write_var:      bsr     search_var
                bne.s   make_var
                move.l  D0,(A0)
                moveq   #0,D1
                rts

make_var:       move.l  A2,-(SP)
                lea     PCRB(var_name),A2
                move.l  (A2),(A1)
                move.l  4(A2),4(A1)
                clr.w   8(A1)
                move.l  D0,(A0)
                movea.l (SP)+,A2
                moveq   #0,D1
                rts

read_var:       movem.l A0-A1,-(SP)
                bsr.s   search_var
                bne.s   readvar1
                move.l  (A0),D1
                movem.l (SP)+,A0-A1
                moveq   #0,D0
                rts
readvar1:       movem.l (SP)+,A0-A1
                moveq   #0,D1
                moveq   #-1,D0
                rts

skip_space:     move.b  (A6)+,D1
                beq.s   skipspa1
                cmp.b   #$0D,D1
                beq.s   skipspa1
                cmpi.b  #' ',D1
                beq.s   skip_space
                cmp.b   #';',D1
                beq.s   skipspa1
                subq.l  #1,A6
                moveq   #0,D0
                rts
skipspa1:       subq.l  #1,A6
                moveq   #-1,D0
                rts


patch_it:       lea     patch_list,A0
                lea     buffer,A1

patchit1:       movem.l (A0)+,D0-D1
                subq.l  #1,D0
                bmi.s   patchit3       ;Ende der Patchliste? Ja! =>
patchit2:       move.b  (A0)+,0(A1,D1.l) ;TOS patchen
                addq.l  #1,D1
                dbra    D0,patchit2
                move.l  A0,D0           ;EVEN
                addq.l  #1,D0
                and.b   #$FE,D0
                movea.l D0,A0
                bra.s   patchit1

patchit3:       rts


****************************************
* write_file: Schreibt ein File auf Disk
* IN A0.l: Zeiger auf den Filename
*    A1.l: Zeiger auf abzuspeichernde Daten
*    D0.l: Anzahl der zu speichernden Bytes

write_file:     movem.l D1-A6,-(SP)
                bsr.s   write
                cmp.l   D5,D3
                bne     rw_err
                bsr.s   rw_ok
                move.l  D3,D0
                movem.l (SP)+,D1-A6
                rts

****************************************
* read_file: Liest ein File bestimmter von Disk
* IN A0.l: Zeiger auf den Filename
*    A1.l: Zeiger auf Datenpuffer
*    D0.l: Anzahl der zu lesenden Bytes

read_file:      movem.l D0-A6,-(SP)
                bsr.s   read
                cmp.l   D5,D3
                bne.s   rw_err
                bsr.s   rw_ok
                movem.l (SP)+,D0-A6
                rts

****************************************
* read_maxfile: Liest ein File beschraenkter Laenge von Disk
* IN A0.l: Zeiger auf den Filename
*    A1.l: Zeiger auf Datenpuffer
*    D0.l: Anzahl der maximal zu lesenden Bytes

read_maxfile:   movem.l D1-A6,-(SP)
                bsr.s   read
                bsr.s   rw_ok
                cmp.l   D5,D3
                blo.s   readmf1
                moveq   #-1,D3
readmf1:        move.l  D3,D0
                movem.l (SP)+,D1-A6
                rts

****************************************
* write/read: Fcreate/Fwrite/Fclose bzw. Fopen/Fread/Fclose
* IN A0.l: Zeiger auf den Filename
*    A1.l: Zeiger auf Datenpuffer
*    D0.l: Anzahl der zu bearbeitenden Bytes

write:          moveq   #$3C,D3         ;Fcreate
                moveq   #$40,D4         ;Fwrite
                bra.s   rw_file

read:           moveq   #$3D,D3         ;Fopen
                moveq   #$3F,D4         ;Fread

rw_file:        move.l  A1,-(SP)        ;File-Puffer
                move.l  D0,-(SP)        ;Anzahl der zu bearbeitenden Bytes
                move.l  D0,D5           ;sichern

                clr.w   -(SP)           ;keine bes. Attribute
                move.l  A0,-(SP)        ;Adresse des Filenames

                lea     PCRD(space_text),A0
                bsr     print_line
                movea.l (SP),A0
                bsr     print_line

                move.w  D3,-(SP)        ;Fcreate/Fopen
                trap    #1              ;GEMDOS 60/61
                addq.l  #8,SP

                move.l  D0,D7           ;Handle merken
                bmi.s   rw_err          ;Fehler? Ja! =>

                move.w  D7,-(SP)        ;Handle
                move.w  D4,-(SP)        ;Fwrite/Fread
                trap    #1              ;GEMDOS 64/63
                lea     12(SP),SP

                move.l  D0,D3           ;Anzahl der bearbeiteten Bytes merken
                bmi.s   rw_err1

                move.w  D7,-(SP)        ;Handle
                move.w  #$003E,-(SP)    ;Fclose
                trap    #1              ;GEMDOS 62
                addq.l  #4,SP

                tst.w   D0
                bmi.s   rw_err

                rts

****************************************
* rw_ok/rw_err: Routinen fuer erfolgreichen bzw. fehlerhaften Abschluss

rw_ok:          lea     PCRD(ok_text),A0
                bra     print_line

rw_err1:        move.w  D7,-(SP)        ;Handle
                move.w  #$003E,-(SP)    ;Fclose
                trap    #1              ;GEMDOS 62
                addq.l  #4,SP

rw_err:         lea     PCRD(error_text),A0
                bsr.s   print_line
                bra     exit

****************************************
* read_length: Bestimmt die Laenge eines Files
* IN A0.l: Zeiger auf den Filename

read_length:    movem.l D1-A6,-(SP)     ;Register retten

                clr.w   -(SP)           ;Nur Lesen
                move.l  A0,-(SP)        ;Zeiger auf Filename
                move.w  #$003D,-(SP)    ;Fopen
                trap    #1              ;GEMDOS 61
                addq.l  #8,SP

                move.w  D0,D7           ;Handle merken
                bmi.s   rl_err          ;Fehler? Ja! =>

                clr.w   -(SP)           ;Ab Anfang
                move.w  D7,-(SP)        ;Handle
                clr.l   -(SP)           ;0 Bytes ueberspringen
                move.w  #$0042,-(SP)    ;Fseek
                trap    #1              ;GEMDOS 66
                lea     10(SP),SP

                move.l  D0,D5           ;Dateianfang merken
                bmi.s   rl_err          ;Fehler? Ja! =>

                move.w  #2,-(SP)        ;Ab Ende
                move.w  D7,-(SP)        ;Handle
                clr.l   -(SP)           ;0 Bytes ueberspringen
                move.w  #$0042,-(SP)    ;Fseek
                trap    #1              ;GEMDOS 66
                lea     10(SP),SP

                move.l  D0,D6           ;Dateiende merken
                bmi.s   rl_err          ;Fehler? Ja! =>

                move.w  D7,-(SP)        ;Handle
                move.w  #$003E,-(SP)    ;Fclose
                trap    #1              ;GEMDOS 62
                addq.l  #4,SP

                tst.w   D0              ;Fehler?
                bmi.s   rl_err1         ;Ja! =>

                sub.l   D5,D6           ;Dateilaenge = Dateiende - Dateianfang
                move.l  D6,D0           ;Dateilaenge nach D0

                movem.l (SP)+,D1-A6     ;Register wiederholen
                rts

rl_err:         move.w  D7,-(SP)        ;Handle
                move.w  #$003E,-(SP)    ;Fclose
                trap    #1              ;GEMDOS 62
                addq.l  #4,SP

rl_err1:        lea     PCRD(space_text),A0
                bsr.s   print_line
                movem.l (SP)+,D1-A6     ;Register wiederholen
                bsr.s   print_line      ;Filename ausgeben
                lea     PCRD(error_text),A0 ;'...Fehler'
                bsr.s   print_line      ;ausgeben
                bra     exit


****************************************
* print_line: Gibt einen Text aus, der mit einem Nullbyte endet
* IN A0.l: Adresse des Textes

print_line:     movem.l D0-A6,-(SP)

                move.l  A0,-(SP)        ;Text ausgeben
                move.w  #9,-(SP)        ;Cconws
                trap    #1              ;GEMDOS 9
                addq.l  #6,SP

                move.w  #$000B,-(SP)    ;Cconis
                trap    #1              ;GEMDOS 11
                addq.l  #2,SP

                tst.w   D0              ;Ein Zeichen verfuegbar?
                beq.s   prline2         ;Nein! => Zurueck

                move.w  #$0008,-(SP)    ;Cnecin
                trap    #1              ;GEMDOS 8
                addq.l  #2,SP

                cmp.b   #$1B,D0         ;War 'ESC' gedrueckt?
                beq.s   abbruch         ;Ja! => Abbruch

prline1:        move.w  #$000B,-(SP)    ;Cconis
                trap    #1              ;GEMDOS 11
                addq.l  #2,SP

                tst.w   D0              ;Ein Zeichen verfuegbar?
                beq.s   prline1         ;Nein! => weiter auf ein Zeichen warten

                move.w  #$0008,-(SP)    ;Cnecin
                trap    #1              ;GEMDOS 8
                addq.l  #2,SP

                cmp.b   #$1B,D0         ;War 'ESC' gedrueckt?
                bne.s   prline2         ;Nein! => Zurueck

abbruch:        pea     PCRD(abbruch_text)
                move.w  #9,-(SP)        ;Cconws
                trap    #1              ;GEMDOS 9
                addq.l  #6,SP
                bra     exit            ;Programm abbrechen

prline2:        movem.l (SP)+,D0-A6     ;Zurueck
                rts

****************************************
* print_point: Gibt einen Punkt aus

print_point:    move.l  A0,-(SP)
                lea     PCRD(point_text),A0
                bsr.s   print_line
                movea.l (SP)+,A0
                rts

****************************************
* hex_out: Gibt eine hexadezimale Zahl aus
* IN D0.l: Auszugebende Zahl

hex_out:        movem.l D0/D6-A1,-(SP)

                lea     PCRB(number_puffer),A0
                movea.l A0,A1
                move.l  #$24303030,(A1)+
                move.l  #$30303030,(A1)+
                move.w  #$3000,(A1)+
                subq.l  #1,A1
                moveq   #7,D7
hex_out1:       move.b  D0,D6
                and.b   #$0F,D6
                add.b   D6,-(A1)
                cmp.b   #10,D6
                blo.s   hex_out2
                addq.b  #'A'-('9'+1),(A1)
hex_out2:       lsr.l   #4,D0
                dbra    D7,hex_out1
                bsr     print_line

                movem.l (SP)+,D0/D6-A1
                rts

****************************************
* dez_out: Gibt eine dezimale Zahl aus
* IN D0.l: Auszugebende Zahl

dez_out:        movem.l D0/D6-A2,-(SP)

                lea     PCRB(number_puffer),A0
                lea     1(A0),A1
                move.l  #$2e300000,(A0)
                lea     PCRD(dez_tab),A2
dez_out0:       move.l  (A2)+,D7
                beq.s   dez_out5
                cmp.l   D7,D0
                blo.s   dez_out0
                moveq   #$30,D6
                bra.s   dez_out2
dez_out1:       moveq   #$30,D6
                move.l  (A2)+,D7
                beq.s   dez_out4
                cmp.l   D7,D0
                blo.s   dez_out3
dez_out2:       addq.b  #1,D6
                sub.l   D7,D0
                cmp.l   D7,D0
                bhs.s   dez_out2
dez_out3:       move.b  D6,(A1)+
                bra.s   dez_out1
dez_out4:       clr.b   (A1)
dez_out5:       bsr     print_line

                movem.l (SP)+,D0/D6-A2
                rts


****************************************
* reloc_it: Reloziert das TOS

reloc_it:       tst.l   reloc_adr
                beq.s   relocite

                lea     PCRD(reloc_text),A0
                bsr     print_line
                move.l  PCRB(reloc_adr),D0
                bsr     hex_out

                bsr.s   optimize
                bsr     reloc
                bsr     forced_reloc

                lea     PCRD(lf_text),A0
                bsr     print_line
relocite:       rts

****************************************
* optimize: Sortiert patch_table

optimize:       lea     PCRB(ptchtab),A6
                movea.l A6,A0

                tst.l   (A0)+
                beq.s   optim3
optim0:         move.l  (A0),D0
                beq.s   optim3
                movea.l A0,A1
optim1:         cmpa.l  A6,A0
                beq.s   optim2
                move.l  -(A0),D1
                move.l  D1,4(A0)
                cmp.l   D1,D0
                blo.s   optim1
optim2:         move.l  D0,4(A0)
                movea.l A1,A0
                addq.l  #4,A0
                bra.s   optim0

optim3:         lea     PCRB(ptchtab2),A6
                movea.l A6,A0

                tst.l   (A0)
                beq.s   optim9
                addq.l  #8,A0
optim4:         move.l  (A0),D0
                beq.s   optim9
                move.l  4(A0),D1
                movea.l A0,A1
optim5:         cmpa.l  A6,A0
                beq.s   optim8
                move.l  -(A0),D3
                move.l  -(A0),D2
                move.l  D2,8(A0)
                move.l  D3,12(A0)

                cmp.l   D1,D2           ;D0/D1 < D2/D3
                bhi.s   optim5
                cmp.l   D3,D0           ;D2/D3 < D0/D1
                bhs.s   optim8

del_entry:      movea.l A1,A2           ;Vergleichanfangs-Eintrag loeschen
delent1:        move.l  8(A2),(A2)+     ;(Rest der Tabelle ein Eintrag nach
                move.l  8(A2),(A2)+     ;vorne kopieren)
                tst.l   (A2)
                bne.s   delent1

                cmp.l   D0,D2
                blo.s   optim6
                cmp.l   D1,D3
                bls.s   optim5       ;D0,D2,D3,D1

                move.l  D3,D1           ;D0,D2,D1,D3
                bra.s   optim5

optim6:         cmp.l   D1,D3
                blo.s   optim7
                move.l  D2,D0           ;D2,D0,D1,D3
                move.l  D3,D1
                bra.s   optim5

optim7:         move.l  D2,D0           ;D2,D0,D3,D1
                bra.s   optim5

optim8:         move.l  D0,8(A0)
                move.l  D1,12(A0)
                movea.l A1,A0
                addq.l  #8,A0
                bra.s   optim4

optim9:         rts

****************************************
* reloc: Reloziert das TOS
* IN base.l        : Basisadresse des TOS
*    length.l      : Laenge des TOS
*    reloc_adr.l   : Adresse, an die das TOS reloziert werden soll
*    ptchtab.l : Adresse der Tabelle mit den Relozierausnahmen
*    ptchtab2.l: Adresse der Tabelle mit den Feld-Relozierausnahmen

reloc:          movea.l PCRB(base),A0
                movea.l A0,A1
                adda.l  PCRB(length),A1

                lea     buffer,A2
                movea.l A2,A3
                adda.l  PCRB(length),A3

                movea.l A2,A4

                lea     PCRB(ptchtab),A5
                lea     PCRB(ptchtab2),A6

                move.w  #$F0FF,D2
                move.w  #$6000,D3
                move.w  #$F0F8,D4
                move.w  #$50C8,D5

                move.l  (A5)+,D6
                move.l  (A6),D7

reloc1:         move.w  (A4)+,D1

                move.w  A4,D0
                sub.w   A2,D0
                bne.s   reloc2

                bsr     print_point

                cmpa.l  A3,A4
                bhs.s   reloc9

reloc2:         move.w  D1,D0
                and.w   D2,D0
                cmp.w   D3,D0
                beq.s   reloc3
                and.w   D4,D0
                cmp.w   D5,D0
                beq.s   reloc3
                swap    D1
                move.w  (A4)+,D1

                move.l  A4,D0
                sub.l   A2,D0
                tst.w   D0
                bne.s   reloc2a

                bsr     print_point

                cmpa.l  A3,A4
                bhs.s   reloc9

reloc2a:        cmp.l   A0,D1
                blo.s   reloc2
                cmp.l   A1,D1
                bhs.s   reloc2

                subq.l  #4,D0           ;wegen 2* .w (A4)+

reloc4:         cmp.l   D6,D0
                blo.s   reloc6
                beq.s   reloc1
reloc5:         move.l  (A5)+,D6
                bne.s   reloc4
                moveq   #-1,D6

reloc6:         cmp.l   D7,D0
                blo.s   reloc7
                cmp.l   4(A6),D0
                blo.s   reloc1
                addq.l  #8,A6
                move.l  (A6),D7
                beq.s   reloc6a
                subq.l  #3,D7
                bra.s   reloc6
reloc6a:        moveq   #-1,D7

reloc7:         sub.l   A0,D1
                add.l   PCRB(reloc_adr),D1
                move.l  D1,-4(A4)
                bra.s   reloc1

reloc3:         addq.l  #2,A4

                move.w  A4,D0
                sub.w   A2,D0
                bne.s   reloc1

                bsr     print_point

                cmpa.l  A3,A4
                blo.s   reloc1
reloc9:         rts

****************************************
* forced_reloc: Reloziert einzelne Adressen
* IN base.l        : Basisadresse des TOS
*    reloc_adr.l   : Adresse, an die das TOS reloziert werden soll
*    forced_table.l: Tabelle mit den zu relozierenden Adressen

forced_reloc:   move.l  PCRB(base),D0
                sub.l   PCRB(reloc_adr),D0
                lea     buffer,A0
                lea     PCRB(forced_table),A3

forcer1:        move.l  (A3)+,D2
                beq.s   forcer2
                add.l   A0,D2
                movea.l D2,A2
                sub.l   D0,(A2)
                bra.s   forcer1
forcer2:        rts


****************************************
* CRC-Checksumme ueber TOS berechnen

crc_checksum:   tst.w   checksum        ;Checksumme berechnen?
                beq     crc10           ;Nein! =>

                lea     PCRD(cksum_text),A0
                bsr     print_line

                move.l  PCRB(width),D7
                lsr.l   #2,D7

                move.l  PCRB(length),D6
                sub.l   D7,D6

                lsr.l   #1,D7
                movea.l D7,A3

                move.l  A3,D0           ;D1 = log 2 (A3)
                moveq   #0,D1
crc1:           lsr.w   #1,D0
                beq.s   crc2
                addq.l  #1,D1
                bra.s   crc1

crc2:           lsr.l   D1,D6
                movea.l D6,A4

                lea     buffer,A2
                lea     PCRD(crctab),A1

                cmpa.w  #1,A3           ;Schrittweite 1?
                bhi.s   crc5            ;Nein! =>

********************
* CRC: einfach

crc3:           movea.l A2,A0
                move.l  A4,D2
                moveq   #0,D0
                moveq   #0,D1

crc4:           move.b  (A0),D3
                adda.l  A3,A0

                eor.b   D3,D1
                add.w   D1,D1
                lea     0(A1,D1.w),A5
                moveq   #0,D1
                move.b  (A5)+,D1
                eor.b   D0,D1
                move.b  (A5),D0

                subq.l  #1,D2
                bne.s   crc4

                move.b  D0,0(A0,A3.l)
                move.b  D1,(A0)

                bsr     print_point

                addq.l  #1,A2
                subq.l  #1,D7
                bne.s   crc3

                bra.s   crc9

********************
* CRC: 2 auf einmal

crc5:           subq.l  #2,A3

crc6:           movea.l A2,A0
                moveq   #0,D0
                moveq   #0,D1
                moveq   #0,D5
                moveq   #0,D6

                moveq   #2,D4           ;2 Durchgaenge fuer (A4) Bytes

crc7:           move.l  A4,D2

crc8:           move.b  (A0)+,D3
                eor.b   D3,D1
                add.w   D1,D1
                lea     0(A1,D1.w),A5
                moveq   #0,D1
                move.b  (A5)+,D1
                eor.b   D0,D1
                move.b  (A5),D0

                move.b  (A0)+,D3
                eor.b   D3,D6
                add.w   D6,D6
                lea     0(A1,D6.w),A5
                moveq   #0,D6
                move.b  (A5)+,D6
                eor.b   D5,D6
                move.b  (A5),D5

                adda.l  A3,A0

                subq.l  #2,D2           ;2 Durchgaenge fuer (A4) Bytes
                bne.s   crc8

                bsr     print_point

                subq.l  #1,D4
                bne.s   crc7

                move.b  D0,2(A0,A3.l)
                move.b  D1,(A0)+

                move.b  D5,2(A0,A3.l)
                move.b  D6,(A0)

                addq.l  #2,A2           ;Anfangsadresse um 2 erhoehen, da
                subq.l  #2,D7           ;2 Durchgaenge auf einmal erledigt
                bne.s   crc6

crc9:           lea     PCRD(lf_text),A0
                bsr     print_line
crc10:          rts


error0:         moveq   #0*4,D0
                bra.s   error_handler
error1:         moveq   #1*4,D0
                bra.s   error_handler
error2:         moveq   #2*4,D0
                bra.s   error_handler
error3:         moveq   #3*4,D0
                bra.s   error_handler
error4:         moveq   #4*4,D0
                bra.s   error_handler
error5:         moveq   #5*4,D0
                bra.s   error_handler
error6:         moveq   #6*4,D0
                bra.s   error_handler
error7:         moveq   #7*4,D0
                bra.s   error_handler
error8:         moveq   #8*4,D0
                bra.s   error_handler
error9:         moveq   #9*4,D0
                bra.s   error_handler
error10:        moveq   #10*4,D0
                bra.s   error_handler
error11:        moveq   #11*4,D0
                bra.s   error_handler
error12:        moveq   #12*4,D0
                bra.s   error_handler
error13:        moveq   #13*4,D0
                bra.s   error_handler
error14:        moveq   #14*4,D0
                bra.s   error_handler
error15:        moveq   #15*4,D0
                bra.s   error_handler
error16:        moveq   #16*4,D0
                bra.s   error_handler
error17:        moveq   #17*4,D0
                bra.s   error_handler
error18:        moveq   #18*4,D0
                bra.s   error_handler
error19:        moveq   #19*4,D0
                bra.s   error_handler
error20:        moveq   #20*4,D0
                bra.s   error_handler
error21:        moveq   #21*4,D0
                bra.s   error_handler
error22:        moveq   #22*4,D0
                bra.s   error_handler
error23:        moveq   #23*4,D0
                bra.s   error_handler
error24:        moveq   #24*4,D0
                bra.s   error_handler
error25:        moveq   #25*4,D0
                bra.s   error_handler
error26:        moveq   #26*4,D0
                bra.s   error_handler
error27:        moveq   #27*4,D0
                bra.s   error_handler
error28:        moveq   #28*4,D0
                bra.s   error_handler
error29:        moveq   #29*4,D0

error_handler:  lea     PCRD(error_tab),A0
                movea.l 0(A0,D0.w),A0
                moveq   #0,D0
                move.b  (A0)+,D0

user_error:     move.w  D0,-(SP)
                bsr     print_line
                move.w  (SP),D0
                and.b   #$7F,D0
                bne.s   errorh2
                lea     PCRD(line_text),A0
                bsr     print_line
                move.l  PCRB(line),D0
                bsr     dez_out
                tst.w   filenr
                beq.s   errorh1
                lea     PCRD(batch2_text),A0
                bsr     print_line
errorh1:        lea     PCRD(point_text),A0
                bsr     print_line
errorh2:        lea     PCRD(lf_text),A0
                bsr     print_line
                move.w  (SP)+,D0
                and.b   #$80,D0
                beq     exit
                lea     PCRD(key2_text),A0
                bsr     print_line
                move.w  #7,-(SP)
                trap    #1
                addq.l  #2,SP
                cmp.b   #$1B,D0
                beq     quit
                rts
__etext:


                DATA

__data: ds.b 0

fname:          DC.B 'TOS.'
fname_ext:      DC.B 'IMG',0
bfname:         DC.B 'PATCH.FIL',0

init_text:      DC.B $1b,'E',$1b,'v',10,$1b,'p TOS-Patch V',version,' ',$1b,'q',13,10,10
                DC.B $bd,'1990    ',$e4,'-soft, Markus Fritze',13,10
                DC.B $bd,'1992-93 Markus Heiden',13,10,10,0
path_text:      DC.B 'Setze Pfad auf "',0
path_end_text:  DC.B '"',13,10,0
read_rom_text:  DC.B 'Lese das TOS aus dem ROM',13,10,0
read_text:      DC.B 'Lese',0
space_text:     DC.B ' ',0
point_text:     DC.B '.',0
lf_text:        DC.B 13,10,0
ok_text:        DC.B '...OK',13,10,0
error_text:     DC.B '...Fehler',13,10,0
line_text:      DC.B ' in Zeile ',0
batch2_text:    DC.B ' der zweiten Batchdatei',0
read_tos:       DC.B 'Lese TOS',13,10,0
write_tos:      DC.B 'Schreibe TOS',13,10,0
reloc_text:     DC.B 'Reloziere TOS nach ',0
cksum_text:     DC.B 'Bilde CRC-Pr',$81,'fsumme',0
error_txt:      DC.B 13,10,'Fehler: ',0
abbruch_text:   DC.B 13,10,10,'Abbruch durch <ESC>...',13,10,0
key_text:       DC.B 13,10,'Ende! Taste dr',$81,'cken.',0
key2_text:      DC.B '--> Bitte Taste dr',$81,'cken! ( <ESC>: Abbruch )',13,10,0


                EVEN

priority:       DC.W 24
                DC.B ')       '
                DC.B $00,')'
                DC.B '<>      '
                DC.B $01,$03
                DC.B '!<>!    '
                DC.B $01,$03
                DC.B '<=      '
                DC.B $01,$02
                DC.B '!<=!    '
                DC.B $01,$02
                DC.B '<       '
                DC.B $01,$01
                DC.B '!<!     '
                DC.B $01,$01
                DC.B '>=      '
                DC.B $01,$05
                DC.B '!>=!    '
                DC.B $01,$05
                DC.B '>       '
                DC.B $01,$04
                DC.B '!>!     '
                DC.B $01,$04
                DC.B '=       '
                DC.B $01,$06
                DC.B '!=!     '
                DC.B $01,$06
                DC.B '!XOR!   '
                DC.B $02,$07
                DC.B '!X!     '
                DC.B $02,$07
                DC.B '!OR!    '
                DC.B $03,$08
                DC.B '!O!     '
                DC.B $03,$08
                DC.B '!AND!   '
                DC.B $04,$09
                DC.B '!A!     '
                DC.B $04,$09
                DC.B '+       '
                DC.B $05,$0A
                DC.B '-       '
                DC.B $05,$0B
                DC.B '*       '
                DC.B $06,$0C
                DC.B '/       '
                DC.B $06,$0D
                DC.B '^       '
                DC.B $07,$0E

dez_tab:        DC.L 1000000000
                DC.L 100000000
                DC.L 10000000
                DC.L 1000000
                DC.L 100000
                DC.L 10000
                DC.L 1000
                DC.L 100
                DC.L 10
                DC.L 1
                DC.L 0

com_tab:        DC.W 5
if_txt:         DC.B 'IF',0,0,0,0,0,0
                DC.L mkplis46
else_txt:       DC.B 'ELSE',0,0,0,0
                DC.L mkplis50
endif_txt:      DC.B 'ENDIF',0,0,0
                DC.L mkplis53
                DC.B 'ERROR',0,0,0
                DC.L mkplis54
                DC.B 'EVEN',0,0,0,0
                DC.L mkplis56

crctab:         DC.W $0000,$1021,$2042,$3063
                DC.W $4084,$50A5,$60C6,$70E7
                DC.W $8108,$9129,$A14A,$B16B
                DC.W $C18C,$D1AD,$E1CE,$F1EF
                DC.W $1231,$0210,$3273,$2252
                DC.W $52B5,$4294,$72F7,$62D6
                DC.W $9339,$8318,$B37B,$A35A
                DC.W $D3BD,$C39C,$F3FF,$E3DE
                DC.W $2462,$3443,$0420,$1401
                DC.W $64E6,$74C7,$44A4,$5485
                DC.W $A56A,$B54B,$8528,$9509
                DC.W $E5EE,$F5CF,$C5AC,$D58D
                DC.W $3653,$2672,$1611,$0630
                DC.W $76D7,$66F6,$5695,$46B4
                DC.W $B75B,$A77A,$9719,$8738
                DC.W $F7DF,$E7FE,$D79D,$C7BC
                DC.W $48C4,$58E5,$6886,$78A7
                DC.W $0840,$1861,$2802,$3823
                DC.W $C9CC,$D9ED,$E98E,$F9AF
                DC.W $8948,$9969,$A90A,$B92B
                DC.W $5AF5,$4AD4,$7AB7,$6A96
                DC.W $1A71,$0A50,$3A33,$2A12
                DC.W $DBFD,$CBDC,$FBBF,$EB9E
                DC.W $9B79,$8B58,$BB3B,$AB1A
                DC.W $6CA6,$7C87,$4CE4,$5CC5
                DC.W $2C22,$3C03,$0C60,$1C41
                DC.W $EDAE,$FD8F,$CDEC,$DDCD
                DC.W $AD2A,$BD0B,$8D68,$9D49
                DC.W $7E97,$6EB6,$5ED5,$4EF4
                DC.W $3E13,$2E32,$1E51,$0E70
                DC.W $FF9F,$EFBE,$DFDD,$CFFC
                DC.W $BF1B,$AF3A,$9F59,$8F78
                DC.W $9188,$81A9,$B1CA,$A1EB
                DC.W $D10C,$C12D,$F14E,$E16F
                DC.W $1080,$00A1,$30C2,$20E3
                DC.W $5004,$4025,$7046,$6067
                DC.W $83B9,$9398,$A3FB,$B3DA
                DC.W $C33D,$D31C,$E37F,$F35E
                DC.W $02B1,$1290,$22F3,$32D2
                DC.W $4235,$5214,$6277,$7256
                DC.W $B5EA,$A5CB,$95A8,$8589
                DC.W $F56E,$E54F,$D52C,$C50D
                DC.W $34E2,$24C3,$14A0,$0481
                DC.W $7466,$6447,$5424,$4405
                DC.W $A7DB,$B7FA,$8799,$97B8
                DC.W $E75F,$F77E,$C71D,$D73C
                DC.W $26D3,$36F2,$0691,$16B0
                DC.W $6657,$7676,$4615,$5634
                DC.W $D94C,$C96D,$F90E,$E92F
                DC.W $99C8,$89E9,$B98A,$A9AB
                DC.W $5844,$4865,$7806,$6827
                DC.W $18C0,$08E1,$3882,$28A3
                DC.W $CB7D,$DB5C,$EB3F,$FB1E
                DC.W $8BF9,$9BD8,$ABBB,$BB9A
                DC.W $4A75,$5A54,$6A37,$7A16
                DC.W $0AF1,$1AD0,$2AB3,$3A92
                DC.W $FD2E,$ED0F,$DD6C,$CD4D
                DC.W $BDAA,$AD8B,$9DE8,$8DC9
                DC.W $7C26,$6C07,$5C64,$4C45
                DC.W $3CA2,$2C83,$1CE0,$0CC1
                DC.W $EF1F,$FF3E,$CF5D,$DF7C
                DC.W $AF9B,$BFBA,$8FD9,$9FF8
                DC.W $6E17,$7E36,$4E55,$5E74
                DC.W $2E93,$3EB2,$0ED1,$1EF0

w_tab:          DC.L w8_ext,w16_ext,w32_ext,w64_ext
w8_ext:         DC.W 'IM',0
w16_ext:        DC.W 'HI','LO',0
w32_ext:        DC.W 'EE','OE','EO','OO',0
w64_ext:        DC.W '0_','1_','2_','3_','4_','5_','6_','7_',0

error_tab:      DC.L error0_t,error1_t,error2_t,error3_t
                DC.L error4_t,error5_t,error6_t,error7_t
                DC.L error8_t,error9_t,error10_t,error11_t
                DC.L error12_t,error13_t,error14_t,error15_t
                DC.L error16_t,error17_t,error18_t,error19_t
                DC.L error20_t,error21_t,error22_t,error23_t
                DC.L error24_t,error25_t,error26_t,error27_t
                DC.L error28_t,error29_t

error0_t:       DC.B 0,'Unerwartetes Zeilen- oder Dateiende',0
error1_t:       DC.B 0,'Illegaler Patchoffset',0
error2_t:       DC.B 0,'Falsches Trennzeichen',0
error3_t:       DC.B $81,'>>> Das TOS ist l',$84,'nger als angegeben!',0
error4_t:       DC.B $81,'>>> Das TOS ist k',$81,'rzer als angegeben!',0
error5_t:       DC.B 1,'Das TOS ist l',$84,'nger als der Puffer!',0
error6_t:       DC.B 1,'Batchdatei zu lang!',0
error7_t:       DC.B 0,$9a,'berlauf',0
error8_t:       DC.B 0,'Illegale Verkn',$81,'pfung',0
error9_t:       DC.B 0,'Division durch 0',0
error10_t:      DC.B 0,'Klammer fehlt',0
error11_t:      DC.B 0,'Wert fehlt',0
error12_t:      DC.B 1,'Zu viele TOS-Dateien zum Splitten!',0
error13_t:      DC.B 0,'Undefinierte Variable',0
error14_t:      DC.B $81,'>>> Falsche TOS-Version!',0
error15_t:      DC.B 0,'Falscher Befehl',0
error16_t:      DC.B 0,'Kein Vergleich nach IF-Befehl',0
error17_t:      DC.B 0,'ELSE ohne IF',0
error18_t:      DC.B 0,'ENDIF ohne IF',0
error19_t:      DC.B 1,'Nur eine zweite Batchdatei gleichzeitig m',$94,'glich!',0
error20_t:      DC.B 0,'IF ohne ENDIF',0
error21_t:      DC.B 1,'TOS-Breite passt nicht zur Anzahl der TOS-Dateien!',0
error22_t:      DC.B 0,'_ERROR-Befehl',0
error23_t:      DC.B 1,'TOS-L',$84,'nge passt nicht zur TOS-Breite!',0
error24_t:      DC.B 1,'Angegebene TOS-L',$84,'nge ist gr',$94,$9e,'er als Puffer!',0
error25_t:      DC.B 1,'TOS-Daten (WIDTH/LENGTH) nicht angegeben!',0
error26_t:      DC.B 0,'Falsches Datumformat',0
error27_t:      DC.B 0,'Patchdatei zu lang',0
error28_t:      DC.B 0,'1. Adresse gr',$94,$9e,'er als 2. Adresse',0
error29_t:      DC.B 0,'Falsche Erweiterung der Variable #',0

                EVEN

__edata: ds.b 0
#define __sdata (__edata-__data)

                BSS
__bss: ds.b 0

base:           DS.L 1
length:         DS.L 1
line:           DS.L 1
filenr:         DS.W 1
checksum:       DS.W 1
output_format:  DS.L 1
width:          DS.L 1
reloc_adr:      DS.L 1
current_adr:    DS.L 1
if_count:       DS.W 1
on_off:         DS.W 1
variable:       DS.L 1
var_name:       DS.B 8
number_puffer:  DS.B 16
varname:        DS.L 1024*2     ;1024 Variablen
varvalue:       DS.L 1024*1
ptchtab:        DS.L 1024       ;1024 Reloc-Korrekturen
ptchtab2:       DS.L 1024*2     ; 512 Feld-Reloc-Korrekturen
forced_table:   DS.L 256        ; 256 Forcierte Reloc-Korrekturen
btchbuf:        DS.B 65536*1    ;64kB Hauptbatchfilepuffer
btchbuf2:       DS.B 32768*1    ;32kB Zweitbatchfilepuffer

                BSS
patch_list:     DS.B max_length/2 ;Platz fuer die Patches
buffer:         DS.B max_length+2 ;Platz fuer das TOS
stack:          DS.B stack_size ;Stack

__ebss: ds.b 0

