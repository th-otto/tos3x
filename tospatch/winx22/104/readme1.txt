
Kurze Erkl�rungen zu "TOSPATCH.TTP" f�r TOS 1.04 (1.02)    Version 2.1

�1990    �-soft, Markus Fritze
�1992-96 Markus Heiden

TosPatch  erm�glicht  das  einfache  �ndern (Patchen) des Tos. F�r das
Einbinden  von  Patches  im  Tos braucht man weder einen Debugger noch
Programme  wie SPLITROM, o.�. Lediglich ein Texteditor wie z.B. Tempus
ist n�tig, um die Parameterdatei zu editieren.


TosPatch ist Freeware!

Das  hei�t, da� das komplette TosPatch-Paket kostenfrei kopiert und in
nicht kommerzielle Mailboxen upgeloadet werden darf. Auch ein Vertrieb
auf  PD-Disketten ist erlaubt, wenn der Diskettenpreis ein Maximum von
DM  10.-   nicht  �bersteigt.  Soll  TosPatch  als  Beilage  zu  einem
kommerziellen Produkt  vertrieben  werden,  so ist sich vorher mit mir
- Markus Heiden -  in Verbindung zu setzen. (Unter nicht kommerziellen
Mailboxen  verstehe ich  Boxen,  die  max.  die  zur Kostendeckung des
Mailboxbetriebs notwendigen Beitr�ge von ihren Usern fordern.)


Haftungsausschlu�:

Trotz  sorgf�ltiger  Arbeit kann die Fehlerfreiheit von TOSPATCH nicht
garantiert  werden.  Somit kann f�r alle Sch�den, die durch Verwendung
von  TosPatch  oder  eines  der  diesem  Paket  enthaltenen  Programme
entstehen  -  gleich  welcher Art und H�he - seitens der Autoren keine
Haftung �bernommen werden.


Beispiele f�r die Anwendung von TosPatch:

-Man  will  ein  TOS  aus  dem  Rechner  auslesen  und  in  eine Datei
 schreiben.   Dazu   ruft   man   TOSPATCH.TTP   mit   dem   Filenamen
 "READROM1.FIL"  auf.  Wenn  das  Programm fertig ist, sollte sich die
 Datei "TOS.IMG" auf dem Datentr�ger befinden.
 ACHTUNG: Das  Programm  unterst�tzt zur Zeit alle TOS-Versionen, d.h.
          die im READROM1.FIL angegebenen TOS-Daten m�ssen stimmen.

-Man will ein "TOS.IMG" in 6 EPROMs brennen. Also TOSPATCH.TTP mit dem
 Filenamen "SPLIT1_6.FIL" aufrufen und wenig sp�ter hat man 6 einzelne
 TOS-Dateien  auf  dem  Datentr�ger. Will man 2 EPROMs haben, gibt man
 als Filenamen "SPLIT1_2.FIL" an.
 Will  man  ein  TOS  gleichzeitig  patchen,  kann man statt dessen im
 Patchfile    die    Zeile   ">[Anzahl der TOS-Files] [Reloc-Adresse]"
 entsprechend anpassen.

-Man  will einige Bytes im nicht gepatchen "TOS.IMG" �ndern. Dazu lade
 man  die  Datei  "PATCH.FIL" in einen Text-Editor seiner Wahl, um das
 Format  zu ergr�nden. Ich hoffe, die umfangreichen Beispiele sprechen
 f�r  sich.  Wie  man  sieht,  kann  man wahlweise einige Bytes direkt
 �ndern,  oder  aber  sogar  ganze  Dateien  includen.  Die Hexzahl am
 Zeilenanfang   entspricht   dabei  dem  Offset  zum  Dateianfang  von
 "TOS.IMG".  Das  "$"  kennzeichnet den Beginn einiger Hexbytes, sonst
 wird  ein  Filename angenommen. Zudem gibt es noch einige zus�tzliche
 M�glichkeiten, welche meist selbsterkl�rend sein sollten. Eine genaue
 Beschreibung ist im File "BEFEHLE.TXT" zu finden.

-Das   Relozieren  funktioniert  nur  mit  dem  originalen,  deutschen
 TOS  1.04  vom 6.4.1989, sonst mu� die  Reloc-Liste angepasst werden.
 Dazu  werden  alle  Langworte, deren  Wert im Bereich $FC0000-$FEFFFF
 liegt,  reloziert.  Die  Reloc-Liste gibt die Ausnahmen der Regel an,
 d.h.  die Langworte, die nicht reloziert werden d�rfen. Die Langworte
 sind  dabei  relative  Offsets zum TOS-Anfang. Um z.B. ein "TOS.IMG",
 d.h.  das  originale,  deutsche  TOS  1.04,  f�r  die Adresse $3D0000
 (z.B.  Mega  4)  zu  relozieren,  ist  einfach  TOSPATCH.TTP  mit dem
 Filenamen "RELOC104.FIL" aufzurufen. Um die Adresse zu �ndern mu� man
 die  Datei  mit einem Text-Editor (wie gehabt) �ndern. Das relozierte
 TOS  kann  man dann mit AUTO_104.PRG starten. Ein so im Ram liegendes
 TOS kann man nat�rlich prima mit einem Debugger (z.B. dem Bugaboo aus
 dem  TurboAss-Paket) bearbeiten (bis zum Absturz...) und neue Patches
 ausprobieren.   Wenn  alles  funktioniert,  sollte  man  die  Patches
 allerdings  in  ASCII-Form  bringen,  damit alles sch�n �bersichtlich
 bleibt (wo habe ich denn was ge�ndert?).

ACHTUNG: Die   Patchfiles,  welche  ein  ausgelesenes  TOS  ben�tigen,
         erwarten dieses als ST_TOS.IMG in dem Verzeichnis, in welchem
         auch    TOSPATCH.TTP   steht.   Die   Ausnahme   bilden   die
         "SPLITRO?.FIL".  Sie erwarten ein TOS.IMG jedoch ebenfalls im
         Verzeichnis, in welchem TOSPATCH.TTP steht.


Welche Vorteile bringt denn nun TosPatch?

1. Man ist nicht mehr gezwungen das TOS.IMG als ganzes weiter zu geben
   (was ja auch zu rechtlichen Problemen f�hren kann).
2. Zudem  kann  man  seine  Patches prima �ber Mailboxen anderen Usern
   zug�nglich machen (wie gesagt, ohne TOS.IMG).
3. Jeder  kann  leicht  erkennen,  was  ge�ndert wird. Dies verringert
   Probleme  bei  Unvertr�glichkeiten  von Patches. Man kann ja leicht
   einen Teil entfernen (z.B. das Setzen das Fast-Load-Bits)
4. Man  kann  sich  schnell  fertige Dateien f�r den Eprommer erzeugen
   lassen.
5. Man kann ein TOS im Ram testen.


Bei Problemen kann man anfragen:

Mail an markus@markusheiden.de


Besonders gelungene Patches sind nat�rlich auch gesucht...

Also: Alle  Modifikationen  an  den  Sourcen und neue Patches BITTE an
      mich  senden.  Auch  Kritik  und  Verbesserungsvorschl�ge, sowie
      Bugreports, sind stets gefragt.


!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!! Bitte wendet euch bez�glich TosPatch nicht mehr an               !!
!! Markus Fritze, sondern ausschlie�lich  an mich (Markus Heiden)!  !!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Viel Spa� w�nscht
   Markus Heiden

   Hamburg, den 14.09.1996

