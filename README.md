
# Aufgabe 1: Ein-/Ausgabe

## Lernziele
1. Kennenlernen der Entwicklungsumgebung
2. Einarbeiten in die Programmiersprache C++
3. Hardwarenahe Programmierung: CGA-Bildschirm und Tastatur


## A1.1: CGA-Bildschirm
Für Testausgaben und zur Erleichterung der Fehlersuche soll das Betriebssystem zunächst Ausgabefunktionen für den Textbildschirm erhalten. Die Funktionsfähigkeit soll mit Hilfe eines aussagefähigen Testprogramms gezeigt werden, siehe Bildschirmfoto unten.

Dazu soll in `main.cc` in der Einstiegsfunktion `main` das Objekt `kout` für verschieden formatierte Ausgaben genutzt werden. Diese sollen ähnlich wie bei der C++ IO-Streams Bibliothek verwendet werden können. Damit die Ausgabefunktionen überall in hhuTOS genutzt werden kann, ist in der gegebenen Klasse `Gobals`, ein globales `CGA_Stream`-Objekt `kout` bereits definiert.

In folgenden Dateien müssen Quelltexte einfügt werden:
`main.cc`, `devices/CGA.cc` und `user/aufgabe1/TextDemo.cc`.

*Beachten Sie die Kommentare im Quelltext der Vorgabe, sowie die Datei* `CGA-slides.pdf`

### Beispielausgaben

![CGA](https://github.com/mschoett/hhuTOSc/blob/aufgabe-1/img/cga.jpg)


## A1.2: Tastatur
Damit eine Interaktion mit dem Betriebssystem möglich wird benötigen wir einen Tastatur-Treiber. In dieser Aufgabe verwenden wir die Tastatur ohne Interrupts. In main soll die Tastatur in einer Endlos-Schleife abgefragt werden und die Eingaben auf dem CGA-Bildschirm zur Kontrolle ausgegeben werden.

Beginnen Sie mit der Funktion `key_hit`:
- Prüfen Sie zunächst in einer Schleife, ob ein Datenbyte von der Tastatur vorliegt. Hierzu muss im Control-Port geprüft werden, ob das Bit `OUTB` gesetzt ist.
- Lesen Sie anschließend das Datenbyte über den Daten-Port ein und speichern Sie das gelesene Byte in der gegebenen Variable code.
- Verwenden Sie die vorgegeben Funktion `key_decoded` um jeweils ein gelesenes Datenbyte zu übersetzen. Jedoch müssen Sie zuvor prüfen, ob das Datenbyte nicht von einer PS/2 Maus stammt. Dies wird über das Bit `AUXB` im Control-Register angezeigt. Beim Aufruf von `key_decoded` müssen Sie das das Datenbyte nicht übergeben, dies ist bereits in der Variablen `code` gespeichert.
- Wenn `key_decoded` true zurückgibt wurde eine Taste komplett dekodiert und in der Variablen `gather` gespeichert. Geben Sie in diesem Fall `gather` (Typ `Key`) zurück oder ansonsten `invalid`.

Danach soll die Funktion `set_repeate_rate` implementiert werden. Zum Schluss können Sie die Funktion `set_led` implementieren (optional).

Namen von benötigten Variablen und Konstanten:
- Control-Port: `ctrl_port`
- Daten-Port: `data_port`
- OUTB: `outb`
- AUXB: `auxb`

Die Befehle für die Implementierung von `set_led` finden Sie in `Keyboard.h`. Warten und prüfen Sie nach dem Absenden eines Befehls die Antwort auf `kbd_reply::ack`.

In folgenden Dateien müssen Quelltexte einfügt werden: `devices/Keyboard.cc` und `user/aufgabe1/KeyboardDemo.cc`.

*Achtung:
Die Methoden zur Ansteuerung der LEDs und der Tastaturwiederholrate funktionieren nur richtig auf echter Hardware.*

*Beachten Sie die Kommentare im Quelltext der Vorgabe, sowie die Dateien* `Tastatur-slides.pdf` *und* `HinweiseTastatur.pdf`.

# Aufgabe 2: Speicherverwaltung und PC-Speaker

## Lernziele
1. Verstehen wie eine Speichervwaltung funktioniert und implementiert wird.
2. Hardwarenahe Programmierung: PC-Speaker / Programmable Interval Timer

## A2.1: Bump-Allocator
In dieser Aufgabe soll ein sehr einfacher sogenannter Bump-Allocator implementiert werden, um zunächst die Integration in das System zu verstehen sowie die Anbindung an die Programmiersprache. Dieser Allokator kennt lediglich den Heap-Anfang, das Heap-Ende und merkt sich in der Variablen `next` die aktuelle Adresse im Heap, ab welcher der Speicher frei ist. Bei jeder Allokation wird `next` um die gewünschte Anzahl Bytes weitergesetzt, sofern nicht das Heap-Ende erreicht ist, siehe Abbildung.

![Bump-Allocator](https://github.com/mschoett/hhuTOSc/blob/aufgabe-2/img/bump_allocator.jpg)

Die Heapgröße ist fest auf 1 MB eingestellt, im Speicherbereich 3 – 4 MB. Bei einer Speicherfreigabe passiert nichts. Bauen Sie die Vorgabe in Ihr System ein und stellen Sie sicher, dass der Heap möglichst bald in der Einstiegsfunktion des Betriebssystems initialisiert wird.

Zur Überprüfung der Implementierung sollen einfache Tests geschrieben werden. Weitere Information hierzu finden sich in den nachfolgenden Hinweisen zur jeweiligen Programmiersprache.

In der Datei `BumpAllocator.cc` soll die Bump-Speicherverwaltung implementiert werden. Die C++
Operatoren für `new` und `delete` sind überschrieben und rufen die entsprechenden Funktionen auf.
In `Globals.cc/.h` gibt es drei neue globale Variablen: `total_mem` wird in `Allocator.cc` gesetzt und enthält die RAM-Kapazität des Rechners (fest auf 8 MB eingestellt). Zudem gibt es für jeden Allokator ein statisches Objekt, für diese Aufgabe BumpAllocator allocator.

In `boot.asm` wird nun der Label `_ZdlPv` auskommentiert, welcher bisher bei einem `delete` angesprungen wurde. Nun wird `delete` im jeweiligen Allokator aufgerufen. Als Tests sollen in `HeapDemo.cc` Instanzen einer eigenen Klasse sowie Arrays von Objekten und/oder primitiven Datentypen angelegt und freigegeben werden.

In folgenden Dateien müssen Quelltexte einfügt werden: `user/aufgabe2/HeapDemo.cc` und `kernel/allocator/BumpAllocator.cc`.


## A2.2: Listenbasierter Allokator
In dieser Aufgabe soll ein verbesserter Allokator implementiert werden, welcher freigegeben Speicherblöcke wiederverwenden kann. Hierzu sollen alle freien Blöcke miteinander verkettet werden, siehe Abbildung.

![List-Allocator](https://github.com/mschoett/hhuTOSc/blob/aufgabe-2/img/list_allocator.jpg)

Zu Beginn gibt es nur einen großen freien Speicherblock, der den gesamten freien Speicher umfasst. Im Rahmen der Heap-Initialisierung soll dieser eine freie Block als erster und einziger Eintrag in der verketteten Freispeicherliste gespeichert werden, siehe Abbildung.

**Allokation**. Bei der Allokation eines Speicherblocks muss die Freispeicherliste nach einem passenden Block durchsucht werden. Es reicht, wenn immer der erste Block genommen wird, der mindestens die Größe der Allokation erfüllt. Sofern der verbleibende Rest groß genug ist, um die Metadaten eines Listeneintrags zu speichern, so soll dieser abgeschnitten und wieder in die Freispeicherliste eingefügt werden.

**Freigabe**. Der freizugebende Block soll in die Freispeicherliste wieder eingehängt werden. Im Prinzip reicht es, wenn er am Anfang der Liste eingefügt wird. Optional kann geprüft werden, ob benachbarte Speicherbereiche auch frei sind und damit verschmolzen werden kann. Dazu muss in der Liste gesucht werden.

Damit die Freispeicherverwaltung getestet und geprüft werden kann, ist es sinnvoll eine Ausgabe-Funktion zu implementieren, welche die Freispeicherliste komplett auf dem Bildschirm ausgibt. Zudem soll die Test-Anwendung aus Aufgabe 2.1 ausgebaut werden, um auch die Freigabe von Speicherblöcken zu testen.

Die folgenden Hinweise sind Ergänzungen zu denen in Aufgabe A2.1!

In der Datei `LinkedListAllocator.cc` soll die Speicherverwaltung implementiert werden. In
`Globals.cc/.h` soll nun LinkedListAllokator allocator verwendet werden.

Es ist zu beachten, dass bei der Allokation neben der angeforderten Speichergröße auch 4 Byte zusätzlich für eine Längenangabe berücksichtigt werden. Dies ist notwendig, damit bei einer Freigabe eines Speicherblocks die Längeninformation verfügbar ist. Es bietet sich an die Länge zu Beginn eines belegten Speicherblocks abzuspeichern und beim Aufruf von `alloc` einen Zeiger direkt nach dem Längenfeld zurückzugeben.

In folgenden Dateien müssen Quelltexte einfügt werden: `user/aufgabe2/HeapDemo.cc` und
`kernel/allocator/LinkedListAllocator.cc`.


## A2.3: PC-Lautsprecher
In dieser Aufgabe muss die Methode `PCPSK::delay` implementiert werden. Diese Methode ist für das Abspielen von Tönen notwendig, die eine gegebene Zeitdauer gespielt werden sollen. Da wir bisher keine Interrupts verarbeiten können und auch keine Systemzeit haben bietet es sich an den Zähler 0 des Programmable Interval Timer (PIT) hierfür zu verwenden. Sie können dann in einer Schleife fortlaufend den aktuellen Zählerstand auslesen, der ja mit 1,19 MHz dekrementiert wird
und so näherungsweise die Ausführung, eine gegebene Zeit in Millisekunden, verzögern. Dies ist eine unsaubere Lösung die wir später ersetzen werden.

Hinweis: gute Informationen zum PIT 8254 finden Sie in der Datei `8254.pdf` sowie hier:
http://wiki.osdev.org/Programmable_Interval_Timer

In folgenden Dateien müssen Quelltexte einfügt werden: `devices/PCSPK.cc` und
`user/aufgabe2/SoundDemo.cc`.


## Beispielausgaben zur Speicherverwaltung
Nachstehend sind einige Screenshots zum Testen der Speicherverwaltung. Sie können sich natürlich selbst Testfunktionen und Testausgaben überlegen. Sollten die Ausgaben über mehrere Seiten gehen bietet es sich an eine Zeitverzögerung mit `pcspk.delay` zu realsieren. Dies ist umständlich und nur als vorübergehende Lösung gedacht; später können wir Tastatureingaben verarbeiten ...

![Heap1](https://github.com/mschoett/hhuTOSc/blob/aufgabe-2/img/heap1.jpg)

![Heap2](https://github.com/mschoett/hhuTOSc/blob/aufgabe-2/img/heap2.jpg)

![Heap3](https://github.com/mschoett/hhuTOSc/blob/aufgabe-2/img/heap3.jpg)

![Heap4](https://github.com/mschoett/hhuTOSc/blob/aufgabe-2/img/heap4.jpg)

# Aufgabe 3: Interrupts

## Lernziele

1. Funktionsweise des Interrupt-Controllers verstehen
2. Behandlung von Interrupts implementieren, am Beispiel der Tastatur
3. Kritische Abschnitte (Synchronisierung) verstehen und einsetzen

## A3.1: Programmable Interrupt Controller (PIC)
In dieser Aufgabe soll die Interrupt-Verarbeitung aktiviert und anhand der Tastatur geprüft werden. Zunächst muss der PIC programmiert werden, um den Interrupt für die Tastatur zuzulassen. Zudem muss der Prozessor Interrupts vom PIC annehmen, was mithilfe der Funktion `cpu.enable_int` gemacht wird. Wenn dies funktioniert, müsste automatisch bei jedem Drücken und Loslassen einer Taste die Funktion `int_disp`, der Interrupt-Dispatcher in C aufgerufen werden, da die Interrupt-Vektortabelle im Assembler-Code (siehe Vorgabe) entsprechend initialisiert wurde.

Mit einer Ausgabe in `int_disp` kann dieser Schritt leicht getestet werden, zumindest einige Male. Wenn die Zeichen nicht vom Tastaturcontroller abgeholt werden, läuft der Tastaturpuffer irgendwann voll. Sobald der Puffer voll ist, sendet der Tastaturcontroller keine Interrupts mehr. Deshalb kann es durchaus passieren, dass zunächst nur für ein oder zwei Tastendrücke Interrupts auftreten.

In der Datei `IntDispatcher.cc` soll in `int_disp eine Textausgabe eingefügt werden.

In `Keyboard.cc` soll in der Methode plugin die ISR der Tastatur in der Klasse `IntDispatcher`
registriert werden sowie in der Klasse `PIC` die Interrupts von der Tastatur freigeschaltet werden. Die Methode `trigger` muss in der Klasse `Keyboard` als leere Methode schon vorhanden sein, damit der Compiler keine Fehlermeldung ausgibt.

In `main.cc` soll plugin von der Tastatur aufgerufen werden und danach die Interruptverarbeitung durch die CPU, mithilfe der CPU-Klasse, freigeschaltet werden.

In folgenden Dateien muss Code implementiert werden: `devices/Keyboard.cc`, und `main.cc`,
`kernel/interupts/PIC.cc` und `kernel/interrupts/IntDispatcher.cc`.


*Allgemeine Hinweise:*
- *Während der Behandlung einer Unterbrechung braucht man sich um unerwünschte Interrupts nicht zu sorgen. Der Prozessor schaltet diese nämlich automatisch aus, wenn er mit der Behandlung beginnt, und lässt sie erst wieder zu, wenn die Unterbrechungsbehandlung beendet wird. Zudem nutzen wir nur einen Prozessor-Kern.*
- *Die Interrupt-Verarbeitung kann nur funktionieren, wenn HHUos auch läuft. Sobald HHUos die main-Funktion verlässt, ist das Verhalten bei Auftreten eines Interrupts undefiniert. Ein Betriebssystem sollte eben nicht plötzlich enden :-)*


**Beispielausgaben in** `int_disp`

![IRQ1](https://github.com/mschoett/hhuTOSc/blob/aufgabe-3/img/irq1.jpg)


## A3.2: Weiterleitung von Interrupts an die Geräte-Treiber
In dieser Aufgabe soll eine Infrastruktur geschaffen werden, um Interrupts, welche in `int_disp` (siehe Aufgabe A3.1) entgegengenommen werden, an eine Interrupt-Service-Routine (ISR) in einem Treiber weiterzuleiten.

Ein Treiber muss hierfür eine ISR implementieren und registrieren. Die Schnittstelle der ISR besteht „nur“ aus der `trigger`-Funktion. Zu beachten ist, dass der Interrupt-Dispatcher mit Vektor-Nummern arbeitet und nicht IRQ-Nummern wie der PIC.

Zur Verwaltung von Treiberobjekten dient die Klasse `IntDispatcher`, die für jeden möglichen
Interrupt einen Zeiger auf ein ISR-Objekt bereithält (verwaltet im Array `map`). Die Schnittstelle für einen Interrupt-Handler wird in der abstrakten Klasse `ISR` definiert. In der neuen Vorgabe erweitert die Klasse `Keyboard` schon die Klasse `ISR`.

In der Klasse `IntDispatcher` soll die Methode assign ein übergebenes ISR-Objekt im Array `map`
registrieren. Die Methode `report` soll von `int_disp` gerufen werden, um die Methode `trigger` einer registrierten ISR-Instanz aufrufen, sofern vorhanden. Falls keine ISR registriert wurde, so soll eine Fehlermeldung ausgegeben werden und das System gestoppt werden.

In der Klasse `Keyboard` soll die Methode plugin die eigene Instanz mithilfe von assign (in der
Klasse `IntDispatcher`) registrieren. Die notwendige Vektor-Nummer ist in `IntDispatcher` definiert. Zudem muss der IRQ der Tastatur (definiert in `PIC.h`) im PIC mithilfe von `allow` freigeschaltet werden.
Des Weiteren soll eine Text-Ausgabe in die Methode `trigger` eingebaut werden, um zu prüfen, ob die Tastaturinterrupts hier ankommen.

In folgenden Dateien muss Code implementiert werden: `devices/Keyboard.cc` und
`kernel/interrupts/IntDispatcher.cc`.

**Beispielausgaben in** `Keyboard::trigger`

![IRQ2](https://github.com/mschoett/hhuTOSc/blob/aufgabe-3/img/irq2.jpg)


## A3.3: Tastaturabfrage per Interrupt
Nun soll die Funktion `trigger` in Keyboard implementiert werden und mithilfe von `key_hit` bei jedem Tastendruck das entsprechende Zeichen an einer festen Position auf dem Bildschirm dargestellt werden. Die Ausgabe des Zeichens soll direkt in `trigger` erfolgen.

Der ASCII-Code der zuletzt gedrückten Taste soll zudem in der neuen Variable `lastkey` gespeichert werden, welche später von Anwendungen ausgelesen werden kann. In `lib/Input.cc` sind zwei Beispielfunktionen welche `lastkey` verwenden, beispielsweise warten bis der Benutzer die Taste Return gedrückt hat.

*Hinweise:*
- *Die PS/2-Maus hängt ebenfalls am Keyboard-Controller, verwendet aber IRQ12. Da wir keinen Handler für IRQ12 haben, kann es sein, dass wenn IRQ1 auftritt noch Daten von der Maus abzuholen sind. Dies können Sie anhand des* `AUXB`*-Bits im Statusregister erkennen.*
- *Ferner tritt unter Qemu manchmal direkt ein IRQ1 nach dem Start auf, ohne eine Tastatureingabe. Das ist auf echter Hardware nicht der Fall. Daher unter Qemu bitte ignorieren.*

**Beispielausgaben in** `Keyboard::trigger` **an einer festen Position**

![IRQ3](https://github.com/mschoett/hhuTOSc/blob/aufgabe-3/img/irq3.jpg)


## A3.4: kritische Abschnitte
Hier soll ein Testprogramm in geschrieben werden. Dieses soll in einer Endlosschleife an einer festen Position Ausgaben machen. Zum Beispiel zeilenweise die Zahlen 0-9.

Es sollte nun möglich sein, durch das Drücken von Tasten die Ausgabe "durcheinander" bringen zu können. Was passiert hier? Wie kann dies vermieden werden?

*Tipp: Für die Synchronisierung / Implementierung eines kritischen Abschnitts gibt es nützliche Funktionen in der Klasse* `CPU`.

**Beispielausgaben "Durcheinander" ohne Synchronisierung**

![IRQ4](https://github.com/mschoett/hhuTOSc/blob/aufgabe-3/img/irq4.jpg)
