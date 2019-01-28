/********************************************************************************
 * TOSLOAD fuer TOS.IMG V2.4                                                    *
 * Dieses kleine Programm laedt ein TOS.IMG (RAM-TOS) an die im TOS             *
 * definierte Adresse. Das TOS wird nur geladen, wenn es nicht im Ram steht.    *
 * Das ermoeglicht es dieses Programm auch in den AUTO-Ordner zu kopieren.      *
 * Dann muss man lediglich TOS.IMG ins Rootdirectory  kopieren.                 *
 * Es ist nun auch moeglich, das TOS korrekt ins TT-Fastram zu laden.           *
 * Man kann das Laden des TOS mit Hotkey (Default: Control) abbrechen.          *
 * Das Ueberladen eines sich bereits im Speicher befindlichden TOS ist moeglich,*
 * wenn der File-Name des TOS in der Commandline uebergeben wird.               *
 * Vor dem Starten des TOS wird die Systemzeit noch neu gesetzt, damit der      *
 * CRC-Test und der Speichertest noch ausgefuehrt werden koennen.               *
 *                                                                              *
 * (C)1990  Sigma-soft, Markus Fritze                                  05.11.90 *
 * (C)1992-93 Markus Heiden                                            20.08.93 *
 * (C)1993    EWK                                                      14.02.93 *
 ********************************************************************************/

#define version '2.4'

                TEXT

                movea.l 4(SP),A5        ;Basepageadresse holen

                pea     cover_msg(PC)   ;'RAM-TOS Loader Vx.x'
                move.w  #9,-(SP)
                trap    #1              ;ausgeben
                addq.l  #6,SP

                moveq   #0,D0
                move.b  128(A5),D0      ;Commandline vorhanden?
                beq     normal          ;Nein! =>
                cmp.b   #126,D0         ;Commandline zu lang?
                bhi     normal          ;Ja! =>


****************************************
* Start mit Commandline-Pfad

                clr.b   -127(A5,D0.w)    ;Ende markieren; BUG: was 129, which is out-of-range for 8-bit displacement

open:           clr.w   -(SP)
                pea     129(A5)         ;Kommandozeile
                move.w  #$3D,-(SP)
                trap    #1              ;Fopen ("TOS.IMG",ReadOnly)
                addq.l  #8,SP

                move.l  D0,D7
                bmi     fnf_error       ;Datei nicht gefunden, o.ae. => raus

get_length:     clr.w   -(SP)           ;Pointer auf Anfang
                move.w  D7,-(SP)        ;der TOS-Datei setzen
                clr.l   -(SP)           ;und Position speichern
                move.w  #$42,-(SP)      ;Fseek
                trap    #1
                lea     10(SP),SP

                move.l  D0,D6           ;Anfangsposition holen (0)
                bmi     load_error      ;Fehler? Ja! =>

                move.w  #2,-(SP)        ;Pointer auf Ende
                move.w  D7,-(SP)        ;der TOS-Datei setzen
                clr.l   -(SP)           ;und Position speichern
                move.w  #$42,-(SP)      ;Fseek
                trap    #1
                lea     10(SP),SP

                tst.l   D0              ;Fehler?
                bmi     load_error      ;Ja! =>

                sub.l   D6,D0           ;Laenge = Ende - Anfang
                move.l  D0,D6

                clr.w   -(SP)           ;Pointer wieder auf Anfang
                move.w  D7,-(SP)        ;der TOS-Datei setzen
                clr.l   -(SP)
                move.w  #$42,-(SP)      ;Fseek
                trap    #1
                lea     10(SP),SP

                tst.l   D0              ;Fehler?
                bmi     load_error      ;Ja! =>

                lea     tos_header(PC),A0 ;Anfang des Puffers
                move.l  4(A5),D4        ;Ende des Speichers
                sub.l   A0,D4           ;Laenge des Speichers
                sub.l   #256,D4         ;Platz fuer den Stack
                bmi     mem_error       ;Fehler, falls nicht genug Speicher

                cmp.l   D4,D6           ;Genug Speicher fuer das ganze TOS?
                bhi     mem_error       ;Nein! =>

                pea     load_message(PC) ;'loading RAM-TOS...'
                move.w  #9,-(SP)
                trap    #1              ;ausgeben
                addq.l  #6,SP

                pea     tos_header(PC)  ;Puffer
                move.l  D6,-(SP)        ;Laenge
                move.w  D7,-(SP)        ;handle
                move.w  #$3F,-(SP)
                trap    #1              ;Fread
                lea     12(SP),SP

                cmp.l   D0,D6           ;alles geladen?
                bne     load_error      ;Nein! =>

                clr.l   -(SP)
                move.w  #$20,-(SP)
                trap    #1              ;Super(0L) - Supervisormode an
                addq.l  #6,SP

own_ttp_stack:  lea     own_ttp_stack(PC),SP ;damit der Stack nicht ueberladen wird

                ori     #$0700,SR       ;Interrupts aus

                lea     tos_header(PC),A0 ;Anfang des TOS
                movea.l 8(A0),A6        ;da soll's hin
                adda.l  D6,A0           ;Ende TOS
                lea     0(A6,D6.l),A1   ;Ende Zielbereich

                lsr.l   #4,D6           ;Anzahl in 4*Longword

copy_loop:      move.l  -(A0),-(A1)     ;16 Bytes kopieren
                move.l  -(A0),-(A1)
                move.l  -(A0),-(A1)
                move.l  -(A0),-(A1)
                subq.l  #1,D6
                bne.s   copy_loop

                bra     tos_start       ;TOS aktivieren


****************************************
* Start aus Autoordner oder ohne Commandline

normal:         clr.w   -(SP)
                pea     fname(PC)       ;Name des TOS (default)
                move.w  #$3D,-(SP)
                trap    #1              ;Fopen ("TOS.IMG",ReadOnly)
                addq.l  #8,SP

                move.l  D0,D7
                bmi     fnf_error       ;Datei nicht gefunden, o.ae. => raus

                moveq   #12,D6          ;12 Bytes lesen
                pea     tos_header(PC)  ;Puffer
                move.l  D6,-(SP)        ;Laenge (12 Bytes)
                move.w  D7,-(SP)        ;handle
                move.w  #$3F,-(SP)
                trap    #1              ;12 Bytes des TOS-Header lesen (Fread)
                lea     12(SP),SP

                cmp.l   D6,D0           ;auch den kompletten Header gelesen?
                bne     load_error      ;Nein => raus, da Fehler

                pea     get_os_base(PC) ;OS-Basisadresse holen
                move.w  #$26,-(SP)      ;Supexec
                trap    #14             ;(XBIOS 38)
                addq.l  #6,SP

                movea.l tos_header+8(PC),A6 ;TOS-Basisadresse holen

                cmpa.l  os_base(PC),A6  ;TOS schon installiert?
                beq     exist_error     ;Ja! => dann raus

                pea     wait(PC)        ;0,5s warten
                move.w  #$26,-(SP)      ;Supexec
                trap    #14             ;(XBIOS 38)
                addq.l  #6,SP

                move.w  #-1,-(SP)       ;Status lesen
                move.w  #11,-(SP)       ;Kbshift
                trap    #13             ;(BIOS 11)
                addq.l  #4,SP

                and.b   defmask(PC),D0  ;mit der Maske verknuepfen
                cmp.b   defbits(PC),D0  ;laden abbrechen?
                beq.s   abbruch

                movem.l tos_header(PC),D0-D1/A6 ;TOS-Header holen
                movem.l D0-D1/A6,(A6)   ;und uebertragen

                pea     load_message(PC) ;'loading RAM-TOS...'
                move.w  #9,-(SP)
                trap    #1              ;ausgeben
                addq.l  #6,SP

                clr.l   -(SP)
                move.w  #$20,-(SP)
                trap    #1              ;Super(0L) - Supervisormode an
                addq.l  #6,SP

own_prg_stack:  lea     own_prg_stack(PC),SP ;damit der Stack nicht ueberladen wird

                pea     12(A6)          ;hinter den Header laden
                move.l  #$7FFFFFFF,-(SP) ;Maximale Anzahl Bytes lesen
                move.w  D7,-(SP)        ;handle
                move.w  #$3F,-(SP)
                trap    #1              ;Fread() : den Rest einlesen
                lea     12(SP),SP

                tst.l   D0
                bmi.s   load_error      ;Datei nicht gefunden, o.ae. => raus


****************************************
* Starten des TOS

tos_start:      move.l  startup_time(PC),$04BA.w ;Timer setzen

                cmpa.l  #$01000000,A6   ;TOS im TT-Ram?
                blo.s   st_ram_start    ;Nein! =>

tt_ram_start:   clr.l   $0426.w         ;Resetvektor ungueltig machen
                move.l  A6,$05A4.w      ;memtop_tt = TOS-Anfang im Ram
                move.l  #$1357BD13,$05A8.w ;memval_tt setzen
                move.l  #$5555AAAA,$051A.w ;memval3 setzen, gibt es bei TOS 1.0 nicht!
                jmp     (A6)            ;und nun RESET ausloesen

st_ram_start:   clr.l   $0426.w         ;Resetvektor ungueltig machen
                move.l  A6,$042E.w      ;phystop = TOS-Anfang im Ram
                move.l  #$5555AAAA,$051A.w ;memval3 setzen, gibt es bei TOS 1.0 nicht!
                jmp     (A6)            ;und nun RESET ausloesen


****************************************
* Fehlerbehandlung

abbruch:        pea     abbruch_msg(PC) ;'Laden abgebrochen'
                bra.s   close

exist_error:    pea     existerr_msg(PC) ;'TOS schon geladen...'
                bra.s   close

fnf_error:      pea     fnferr_msg(PC) ;'TOS-Datei nicht gefunden!'
                bra.s   error_out

mem_error:      pea     memerr_msg(PC) ;'Nicht genug RAM vorhanden!'
                bra.s   close

load_error:     pea     loaderr_msg(PC) ;'Fehler beim Laden der TOS-Datei!'

close:          move.w  D7,-(SP)        ;handle
                move.w  #$3E,-(SP)      ;Fclose
                trap    #1              ;Tos-Datei schliessen
                addq.l  #4,SP

error_out:      move.w  #9,-(SP)
                trap    #1              ;Message ausgeben
                addq.l  #6,SP

                clr.w   -(SP)           ;Pterm0
                trap    #1              ;weiter mit dem AUTO-Ordner

                illegal                 ;Abbruch, falls Gemdos doch zurueckkehrt


****************************************
* Supervisor-Subroutinen

get_os_base:    movea.l $04F2.w,A0      ;Systemheaderadresse holen
                move.l  8(A0),os_base   ;OS-Basisadresse holen
                rts                     ;Fertig

wait:           moveq   #100,D0         ;0,5s warten
                add.l   $04BA.w,D0
wait1:          cmp.l   $04BA.w,D0
                bhi.s   wait1
                rts                     ;Fertig


****************************************
* Datenbereich

                DATA

abbruch_msg:    DC.B ' RAM-TOS wird nicht geladen!',13,10,10,0
load_message:   DC.B ' loading RAM-TOS...',13,10,10,0
loaderr_msg:    DC.B ' Fehler beim Laden der TOS-Datei!',13,10,10,0
fnferr_msg:     DC.B ' TOS-Datei nicht gefunden!',13,10,10,0
memerr_msg:     DC.B ' Nicht genug RAM vorhanden!',13,10,10,0
existerr_msg:   DC.B ' TOS schon geladen...',13,10,10,0
cover_msg:      DC.B 13,10,27,'p RAM-TOS Loader V',version,' ',27,'q',13,10,0

                DC.B ' >>> ',$bd,'1990 ',$e4,'-soft, Markus Fritze /'
                DC.B ' ',$bd,'1992-93 Markus Heiden /'
                DC.B ' ',$bd,'1993 EWK <<< '

                EVEN

                DC.B ' PATCH HERE:'
defmask:        DC.B $04        ;Hotkey-Maske
defbits:        DC.B $04        ;Hotkey
startup_time:   DC.L 0          ;Systemzeit beim Starten des TOS
                DS.B 10         ;Platz fuer zukuenftige Flags
fname:          DC.B 'TOS.IMG'  ;Pfad + File-Namen des Default-TOS
                DS.B 128-7


****************************************
* Variablenbereich

                BSS

                EVEN

os_base:        DS.L 1
tos_header:     DS.B 12
