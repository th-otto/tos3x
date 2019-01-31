
Kurze ErklÑrungen zu "TOSPATCH.TTP" fÅr TOS 3.06           Version 2.0

Ω1990    ‰-soft, Markus Fritze
Ω1992-97 Markus Heiden

TosPatch  ermîglicht  das  einfache  éndern (Patchen) des Tos. FÅr das
Einbinden  von  Patches  im  Tos braucht man weder einen Debugger noch
Programme  wie SPLITROM, o.Ñ. Lediglich ein Texteditor wie z.B. Tempus
ist nîtig, um die Parameterdatei zu editieren.


TosPatch ist Freeware!

Das  heiût, daû das komplette TosPatch-Paket kostenfrei kopiert und in
nicht kommerzielle Mailboxen upgeloadet werden darf. Auch ein Vertrieb
auf  PD-Disketten ist erlaubt, wenn der Diskettenpreis ein Maximum von
DM  10.-   nicht  Åbersteigt.  Soll  TosPatch  als  Beilage  zu  einem
kommerziellen Produkt  vertrieben  werden,  so ist sich vorher mit mir
- Markus Heiden -  in Verbindung zu setzen. (Unter nicht kommerziellen
Mailboxen  verstehe ich  Boxen,  die  max.  die  zur Kostendeckung des
Mailboxbetriebs notwendigen BeitrÑge von ihren Usern fordern.)


Haftungsausschluû:

Trotz  sorgfÑltiger  Arbeit kann die Fehlerfreiheit von TosPatch nicht
garantiert  werden.  Somit kann fÅr alle SchÑden, die durch Verwendung
von  TosPatch  oder  eines  der  diesem  Paket  enthaltenen  Programme
entstehen  -  gleich  welcher Art und Hîhe - seitens der Autoren keine
Haftung Åbernommen werden.


Beispiele fÅr die Anwendung von TosPatch:

-Man  will  ein  TOS  aus  dem  Rechner  auslesen  und  in  eine Datei
 schreiben.   Dazu   ruft   man   TOSPATCH.TTP   mit   dem   Filenamen
 "READROM3.FIL"  auf.  Wenn  das  Programm fertig ist, sollte sich die
 Datei "TOS.IMG" auf dem DatentrÑger befinden.
 ACHTUNG: Das  Programm  unterstÅtzt zur Zeit alle TOS-Versionen, d.h.
          die im READROM3.FIL angegebenen TOS-Daten mÅssen stimmen.

-Man will ein "TOS.IMG" in 2 EPROMs brennen. Also TOSPATCH.TTP mit dem
 Filenamen "SPLIT3_2.FIL" aufrufen und wenig spÑter hat man 2 einzelne
 TOS-Dateien  auf  dem  DatentrÑger. Will man 4 EPROMs haben, gibt man
 als Filenamen "SPLIT3_4.FIL" an oder "SPLIT3_8.FIL" fÅr 8 EPROMs.
 Will  man  ein  TOS  gleichzeitig  patchen,  kann man statt dessen im
 Patchfile    die    Zeile   ">[Anzahl der TOS-Files] [Reloc-Adresse]"
 entsprechend anpassen.

-Man  will einige Bytes im nicht gepatchen "TOS.IMG" Ñndern. Dazu lade
 man  die Datei "306_REL.FIL" in einen Text-Editor seiner Wahl, um das
 Format  zu ergrÅnden. Ich hoffe, die umfangreichen Beispiele sprechen
 fÅr  sich.  Wie  man  sieht,  kann  man wahlweise einige Bytes direkt
 Ñndern,  oder  aber  sogar  ganze  Dateien  includen.  Die Hexzahl am
 Zeilenanfang   entspricht   dabei  dem  Offset  zum  Dateianfang  von
 "TOS.IMG".  Das  "$"  kennzeichnet den Beginn einiger Hexbytes, sonst
 wird  ein  Filename angenommen. Zudem gibt es noch einige zusÑtzliche
 Mîglichkeiten, welche meist selbsterklÑrend sein sollten. Eine genaue
 Beschreibung ist im File "BEFEHLE.TXT" zu finden.

-Das   Relozieren  funktioniert  nur  mit  dem  originalen,  deutschen
 TOS 3.06  vom 24.09.1991, sonst muû die Reloc-Liste angepasst werden.
 Dazu  werden  alle  Langworte,  deren Wert im Bereich $E00000-$E7FFFF
 liegt,  reloziert.  Die  Reloc-Liste gibt die Ausnahmen der Regel an,
 d.h.  die Langworte, die nicht reloziert werden dÅrfen. Die Langworte
 sind  dabei  relative  Offsets zum TOS-Anfang. Um z.B. ein "TOS.IMG",
 d.h.  das  originale,  deutsche  TOS 3.06  fÅr  die  Adresse  $380000
 (z.B.  Mega  ST  4)  zu  relozieren, ist einfach TOSPATCH.TTP mit dem
 Filenamen  "RELOC306.FIL"  aufzurufen.  Um die Adresse zu Ñndern, muû
 man  die  Datei  mit  einem  Text-Editor  (wie  gehabt)  Ñndern.  Das
 relozierte  TOS  kann man dann mit LOADTOS.PRG starten. Ein so im Ram
 liegendes  TOS  kann man natÅrlich prima mit einem Debugger (z.B. dem
 Bugaboo  aus  dem TurboAss-Paket) bearbeiten (bis zum Absturz...) und
 neue  Patches  ausprobieren.  Wenn alles funktioniert, sollte man die
 Patches   allerdings   in   ASCII-Form  bringen,  damit  alles  schîn
 Åbersichtlich bleibt (wo habe ich denn was geÑndert?).

ACHTUNG: Die   Patchfiles,  welche  ein  ausgelesenes  TOS  benîtigen,
         erwarten dieses als TT_TOS.IMG in dem Verzeichnis, in welchem
         auch    TOSPATCH.TTP   steht.   Die   Ausnahme   bilden   die
         "SPLITRO?.FIL".  Sie erwarten ein TOS.IMG jedoch ebenfalls im
         Verzeichnis, in welchem TOSPATCH.TTP steht.


Welche Vorteile bringt denn nun TosPatch?

1. Man ist nicht mehr gezwungen das TOS.IMG als ganzes weiter zu geben
   (was ja auch zu rechtlichen Problemen fÅhren kann).
2. Zudem  kann  man  seine  Patches prima Åber Mailboxen anderen Usern
   zugÑnglich machen (wie gesagt, ohne TOS.IMG).
3. Jeder  kann  leicht  erkennen,  was  geÑndert wird. Dies verringert
   Probleme  bei  UnvertrÑglichkeiten  von Patches. Man kann ja leicht
   einen Teil entfernen (z.B. das Setzen das Fast-Load-Bits)
4. Man  kann  sich  schnell  fertige Dateien fÅr den Eprommer erzeugen
   lassen.
5. Man kann ein TOS im Ram testen.


Bei Problemen kann man anfragen:

Mail an markus@markusheiden.de


Besonders gelungene Patches sind natÅrlich auch gesucht...

Also: Alle  Modifikationen  an  den  Sourcen und neue Patches BITTE an
      mich  senden.  Auch  Kritik  und  VerbesserungsvorschlÑge, sowie
      Bugreports, sind stets gefragt.


!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!! Bitte wendet euch bezÅglich TosPatch nicht mehr an               !!
!! Markus Fritze, sondern ausschlieûlich  an mich (Markus Heiden)!  !!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Viel Spaû wÅnscht
     Markus Heiden

   Hamburg, den 19.02.1997

