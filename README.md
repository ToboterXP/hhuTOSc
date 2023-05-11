
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
In dieser Aufgabe soll die Interrupt-Verarbeitung aktiviert und anhand der Tastatur geprüft werden.

Zunächst müssen die leeren Methoden in der Klasse `PIC` implementiert werden.  In der Funktion `int_disp` (in `IntDispatcher.cc`) soll vorerst eine Textausgabe eingefügt werden, welche ausgibt, dass ein Interrupt aufgetreten ist und welche Vektor-Nummer dieser hat.

Anschliessend soll die Methode `plugin` in `Keyboard.cc` programmiert werden. Hier muss der Interrupt der Tastatur am PIC mit `pic.allow` freigeschaltet werden. Die Methode `trigger` kann vorerst leer bleiben, muss aber schon vorhanden sein.

In `main` muss die ISR der Tastatur mit `kb.plugin` registriert werden und danach muessen die Interrupts an der CPU  mit `cpu.enable_int()` zugelassen werden. In der Vorgabe wird der PIC bereits durch Aufruf von `init_interrupts()` initialisiert.

Wenn nun das System startet sollte bei jedem Drücken und Loslassen einer Taste eine Textmeldung von `int_disp` zu sehen sein. Dies funktioniert allerdings nu einige wenige Male. Wenn die Zeichen nicht vom Tastaturcontroller abgeholt werden, läuft der Tastaturpuffer irgendwann voll. Sobald der Puffer voll ist, sendet der Tastaturcontroller keine Interrupts mehr.
In folgenden Dateien muss Code implementiert werden: `devices/Keyboard.cc`, und `main.cc`,`kernel/interupts/PIC.cc` und `kernel/interrupts/IntDispatcher.cc`.


*Allgemeine Hinweise:*- *Während der Behandlung einer Unterbrechung braucht man sich um unerwünschte Interrupts nicht zu sorgen. Der Prozessor schaltet diese nämlich automatisch aus, wenn er mit der Behandlung beginnt, und lässt sie erst wieder zu, wenn die Unterbrechungsbehandlung beendet wird. Zudem nutzen wir nur einen Prozessor-Kern.*- *Die Interrupt-Verarbeitung kann nur funktionieren, wenn hhuTOS auch läuft. Sobald hhuTOS die* `main`*-Funktion verlässt, ist das Verhalten bei Auftreten eines Interrupts undefiniert. Ein Betriebssystem sollte eben nicht plötzlich enden :-)*


**Beispielausgaben in** `int_disp`

![IRQ1](https://github.com/mschoett/hhuTOSc/blob/aufgabe-3/img/irq1.jpg)


## A3.2: Weiterleitung von Interrupts an die Geräte-Treiber
In dieser Aufgabe soll eine Infrastruktur geschaffen werden, um Interrupts, welche in `int_disp` (siehe Aufgabe A3.1) entgegengenommen werden, an eine Interrupt-Service-Routine (ISR) in einem Treiber weiterzuleiten.

Zur Verwaltung von Treiberobjekten dient die Klasse `IntDispatcher`, die für jeden möglichenInterrupt einen Zeiger auf ein ISR-Objekt bereithält (verwaltet im Array `map`). Die Schnittstelle für einen Interrupt-Handler wird in der abstrakten Klasse `ISR` definiert. In der neuen Vorgabe erweitert die Klasse `Keyboard` schon die Klasse `ISR`.
In der Klasse `IntDispatcher` soll die Methode assign ein übergebenes ISR-Objekt im Array `map`registrieren. Die Methode `report` soll von `int_disp` gerufen werden, um die Methode `trigger` einer registrierten ISR-Instanz aufrufen, sofern vorhanden. Falls keine ISR registriert wurde, so soll eine Fehlermeldung ausgegeben werden und das System gestoppt werden.
In der Klasse `Keyboard` soll die Methode `plugin` erweitert werden und die eigene Instanz mithilfe von `assign` (in der Klasse `IntDispatcher`) registrieren. Die notwendige Vektor-Nummer ist in `IntDispatcher` definiert.

Des Weiteren soll eine Text-Ausgabe in die Methode `trigger` eingebaut werden, um zu prüfen, ob die Tastaturinterrupts hier ankommen.

In folgenden Dateien muss Code implementiert werden: `devices/Keyboard.cc` und
`kernel/interrupts/IntDispatcher.cc`.

**Beispielausgaben in** `Keyboard::trigger`

![IRQ2](https://github.com/mschoett/hhuTOSc/blob/aufgabe-3/img/irq2.jpg)


## A3.3: Tastaturabfrage per Interrupt
Nun soll die Funktion `trigger` in Keyboard implementiert werden. Bei jedem Interrupt soll `key_hit` aufgerufen und geprüft werden, ob ein Zeichen erfolgreich dekodiert wurde. Wenn dies der Fall ist, so soll der ASCII-Code des Zeichens in der neuen Variable `lastkey` gespeichert werden, welche später von Anwendungen ausgelesen werden kann. In `lib/Input.cc` sind zwei Beispielfunktionen welche `lastkey` verwenden, beispielsweise warten bis der Benutzer die Taste Return gedrückt hat.

Zudem soll `trigger` ein erfolgreich dekodiertes Zeichen an einer festen Position (mit `kout.setpos()`) auf dem Bildschirm anzeigen.
*Hinweise:*- *Die PS/2-Maus hängt ebenfalls am Keyboard-Controller, verwendet aber IRQ12. Da wir keinen Handler für IRQ12 haben, kann es sein, dass wenn IRQ1 auftritt noch Daten von der Maus abzuholen sind. Dies können Sie anhand des* `AUXB`*-Bits im Statusregister erkennen.*- *Ferner tritt unter Qemu manchmal direkt ein IRQ1 nach dem Start auf, ohne eine Tastatureingabe. Das ist auf echter Hardware nicht der Fall. Daher unter Qemu bitte ignorieren.*

**Beispielausgaben in** `Keyboard::trigger` **an einer festen Position**

![IRQ3](https://github.com/mschoett/hhuTOSc/blob/aufgabe-3/img/irq3.jpg)


## A3.4: kritische Abschnitte
Es soll ein Testprogramm in geschrieben werden, welches in einer Endlosschleife an einer festen Position Text-Ausgaben mach, zeilenweise die Zahlen 0 - 9.
Es sollte nun möglich sein, durch das Drücken von Tasten die Ausgabe "durcheinander" bringen zu können. Was passiert hier? Wie kann dies vermieden werden?*Tipp: Für die Synchronisierung / Implementierung eines kritischen Abschnitts gibt es nützliche Funktionen in der Klasse* `CPU`.

**Beispielausgaben "Durcheinander" ohne Synchronisierung**

![IRQ4](https://github.com/mschoett/hhuTOSc/blob/aufgabe-3/img/irq4.jpg)
# Aufgabe 4: Koroutinen und Threads

## Lernziele
1. Auffrischen der Assemblerkenntnisse
2. Verständnis der Abläufe bei einem Koroutinen-Wechsel
3. Unterschied zwischen Threads und Koroutinen
3. Verstehen wie ein Scheduler funktioniert

FÜr diese Aufgabe sollte zuvor der Assembler-Crashkurs in `ASM-slides.pdf` gelesen werden.

## A4.1: Koroutinen
In dieser Aufgabe soll die Umschaltung zwischen Koroutinen in Assembler programmiert werden. Koroutinen sind eine Vorstufe zu Threads die später (siehe unten) zusätzlich eingeführt werden.

Sehen Sie sich zunächst die Inhalte der neuen Dateien in der Vorgabe im Ordner `kernel/corouts` an und implementieren Sie die beiden Assemblerfunktionen `coroutine_start` und `coroutine_switch` in `coroutine.asm`. Der Zustand (alle Register) einer Koroutine soll auf dem Stack gesichert werden. Das `rflags`-Register kann nicht direkt per move-Befehl zugegriffen werden, sondern nur mithilfe der Instruktionen `popf` und `pushf`.

Der Zeiger auf den letzten Stack-Eintrag soll in der Instanzvariablen `context` in der Klasse `Coroutine` gespeichert werden.

Ergänzen Sie anschließend die leeren Methoden in `Coroutine.cc`. Die Verkettung der Koroutinen erfolgt durch Erben von der Klasse `lib/Chain`.

Schreiben Sie für Ihre Koroutinen-Implementierung folgendes Testprogramm. Im Verzeichnis
`user` der Vorgabe finden Sie hierfür Dateien. Es sollen drei Koroutinen erzeugt und miteinander
verkettet werden. Jede Koroutine soll einen Zähler hochzählen und an einer festen Position auf dem Bildschirm ausgeben und dann auf die nächste Koroutine umschalten. Durch die Verkettung werden die drei Koroutinen dann reihum abwechselnd ausgeführt, wodurch die Zähler scheinbar nebenläufig vorangetrieben werden, siehe nachstehende Abbildung.

In folgenden Dateien muss Code implementiert werden: `kernel/corouts/Coroutine.asm`, `kernel/corouts/Coroutine.cc`, `user/aufgabe4/CoroutineDemo.cc`, `user/aufgabe4/CoroutineLoop.cc` und `main.cc`

Hinweis: Schauen Sie sich vor dem Programmieren der Assemblerfunktionen nochmals die Aufrufkonvention für die Parameterübergabe an.

![KOR1](https://github.com/hhu-bsinfo/hhuTOSc/blob/aufgabe-4/img/corouts.jpg)

## A4.2: Warteschlange
Der Scheduler benötigt eine Warteschlange (engl. queue) bei der immer am Anfang einer einfach verketteten Liste ein Element entfernt wird (Thread der als nächstes die CPU erhält) und immer Ende eingefügt wird (zum Beispiel ein neuer Thread oder ein Thread der die CPU abgibt).

Implementieren Sie eine Warteschlange die diese Funktionalität realisiert im Verzeichnis `lib`. Testen Sie diese Klasse bevor Sie die nächste Aufgabe beginnen mit einem eigenen Testprogramm, außerhalb des Betriebssystems.

## A4.3: Umbau der Koroutinen auf Threads
Kopieren Sie das Unterverzeichnis `kernel/corouts` um nach `kernel/threads` und benennen Sie danach die Dateien im Verzeichnis `kernel/threads` wie folgt um. Passen Sie dann die Namen der Klassen, Konstruktoren, Methoden und Funktionen in den obigen Dateien entsprechend an und ersetzen den Namen *Coroutine* durch den Namen *Thread*.

Umzukopieren sind folgende Dateien:
- Coroutine.asm -> Thread.asm
- Coroutine.cc  -> Thread.cc

Modifizieren Sie nun `Thread.cc` passend zur der Datei `Thread.h` in der Vorgabe.
- Die Methode `setNext` soll gelöscht werden.
- Die Methode `switchToNext` soll durch `switchTo` (siehe Vorgabe `Thread.h`) ersetzt werden. Die Semantik bleibt gleich, aber die Threads sind nicht fest miteinander verkettet, wie die Koroutinen. Daher wird der nächste Thread in `next` als Parameter übergeben.
- Im Konstruktor soll eine eindeutige Thread-ID vergeben werden, dies kann mithilfe einer globalen Variablen in `Thread.cc` realisiert werden. Zudem soll im Konstruktor der Stack (4 KB sind ausreichend) mithilfe ihrer Speicherverwaltung dynamisch angelegt werden.

*Hinweis: Diese Aufgabe kann nicht separat getestet werden.*


## A4.4 Scheduler
Nun soll ein einfacher Scheduler implementiert werden. Alle Threads werden in einer Bereit-Wartschlange (siehe A4.2) verwaltet und bekommen reihum die CPU (nach freiwilliger Abgabe mittels `yield`). Es gibt keine Prioritäten und es ist sinnvoll, dass der aktuell laufende Thread nicht in der Warteschlange gespeichert wird. In der Vorgabe ist die Implementierung für den Idle-Thread gegeben, welcher läuft, falls kein Anwendungsthread in der Bereit-Warteschlange ist. Der Idle-Thread soll im Rahmen der Initialisierung des Schedulers erzeugt und registriert werden, siehe A4.5.

Testen Sie den Scheduler zunächst nur mit dem Idle-Thread. Bauen Sie hierzu eine Textausgabe in den Idle-Thread ein.

Gegeben ist in der Vorgabe die Klasse `Dispatcher`. Hier wird der aktuell laufende Thread verwaltet, sowie das Umschalten auf einen anderen Thread mithilfe der `switchTo`-Methode der Klasse `Thread` realisiert. In der Klasse `Dispatcher` ist auch eine Methode `start` zum Anstoßen des ersten Threads (mithilfe ihrer Assembler-Funktion).

In der gegebenen Datei `Scheduler.cc` sind die gekennzeichneten Methoden zu implementieren. Bei
einem Thread-Wechsel soll der Thread am Kopf der `readyQueue` entfernt und durch `life` (in
`Disptacher.cc`) referenziert. Gibt ein Thread die CPU freiwillig durch Aufruf von `yield` ab, soll dieser Thread wird wieder am Ende der `readyQueue` eingefügt werden.


## A4.5 Eine multi-threaded Testanwendung
Die Vorgabe beinhaltet einen HelloWorld-Thread, um einen ersten Test durchzuführen. Der Thread gibt einen Spruch aus und terminiert sich dann. Anschließend soll nur noch der Idle-Thread ausgeführt werden. Um dies zu testen soll der Idle-Thread und der HelloWorld-Thread in `main` angelegt und im Scheduler registriert werden. Anschließend soll der Scheduler mit `scheduler.schedule()` gestartet werden.

Als zweiter eigener Test soll nun eine multi-threaded Testanwendung bestehend aus vier Threads geschrieben werden. Hierzu soll das Anwendungsbeispiel mit den drei Zählern vom letzten Übungsblatt von Koroutinen auf Threads umgebaut werden. Ein Haupt-Thread der Anwendung `CoopThreadDemo` erzeugt drei Zähler-Threads, Instanzen von der Klasse `LoopThread`. Der Haupt-Thread der Anwendung soll eine gewisse Zeit laufen und sich dann selbst mit exit terminieren, nachdem er beispielsweise 1000 Mal die CPU mit `yield`abgegeben hat. Bevor sich der Haupt-Thread der Anwendung terminiert soll er noch einen `LoopThread` mit `kill` terminieren. Somit sollten zunächst drei Zähler auf dem Bildschirm ausgegeben werden und dann einer bei 1000 stoppen, siehe Abbildung unten.

**Beispielausgaben der Threads**

![THR](https://github.com/hhu-bsinfo/hhuTOSc/blob/aufgabe-4/img/threads.jpg)

# Aufgabe 5: Präemptives Multithreading

## Lernziele
- Tieferes Verständnis von präemptiven Multitasking
- CPU-Entzug mithilfe des PIT
- Synchronisierung des Schedulers gegenüber dem PIT-Interrupt


## A5.1: Programmable Interval Timer (PIT)Der PIT wird ab sofort verwendet, um eine Systemzeit sowie ein erzwungenes Umschalten zwischen Threads zu realisieren. Die Systemzeit wird in der globalen Variable `systime` (in `Globals.cc`) gespeichert und diese wird bei jedem Interrupt für den PIT inkrementiert. Verwenden Sie hierfür im PIT den Channel 0 und Modus 3 und laden Sie den Zähler mit einem passenden Wert, sodass der PIT alle 10ms ein Interrupt ausgelöst. Jeder Interrupt verursacht also eine Inkrementierung und entspricht einem Tick (10ms). Somit zeigt `systime` an, wie viele Ticks seit dem Beginn der Zeiterfassungvergangen sind.
Geben Sie im Interrupt-Handler des PIT den Fortschritt der Systemzeit an einer festen Stelle aus, jedoch nicht bei jedem Tick, sondern nur alle 100 Ticks. Verwenden Sie hierfür beispielsweise die rechte obere Ecke und folgende Zeichen |, /, -, \, wobei das Zeichen bei jeder Ausgabe wechselt. Dadurch, dass ein Zeichenausgabe nur alle 100 Ticks erfolgt, ändert sich das Zeichen ungefähr jede Sekunde.

Testen Sie die Klasse PIT indem Sie die ISR für den PIT in main aktivieren und die Ausgabe der Systemzeit überprüfen.
Für den PIT gibt es in `Globals.cc`eine globale Variable `pit` und für die Systemzeit die Variabele `systime`.

In folgenden Dateien muss Code implementiert werden: `devices/PIT.cc` und `main.cc`.


## A5.2: Umbau des Treibers für den PC-Lautsprecher
Die `delay` Funktion im Treiber für den PC-Lautsprecher hat bisher den PIT direkt verwendet und soll nun die Systemzeit aus A5`1 nutzen. Hierzu soll eine Busy-Waiting-Schleife verwendet werden, welche stoppt, wenn die `systime` entsprechend der gewünschten Verzögerung fortgeschritten ist. Busy-Waiting ist nicht schön, aber durch das präemptive Multithreading akzeptabel.

Testen Sie den Umbau mit einer der Melodien.

In folgender Datei muss Code implementiert werden: `devices/PCSPK.cc`.

## A5.3 Threadumschaltung mithilfe des PIT
In der Vorgabe ist neu die Methode `preempt`in `Scheduler.cc`. Diese Methode soll bei jedem Tick aus der ISR vom PIT aufgerufen werden und eine erzwungene Threadumschaltung durchführen. Somit entspricht eine Zeitscheibe einem Tick. Das Umschalten kann mithilfe der Methode `dispatch`erfolgen, wie bisher bei `yield`.

Zusätzliche müssen die Methoden des Schedulers mithilfe von Interrupt-Sperren gegenüber dem PIT synchronisiert werden. Ansonsten kann es sein, dass die Ready-Queue kaputt geht, wenn in einem ungünstigen Augenblick `preempt`aufgerufen wird.

Zudem muss sichergestellt werden, dass der Scheduler fertig initialisiert ist, bevor das erste Mal `preempt`versucht umzuschalten. Dies kann am einfachsten mithilfe einer Instanzvariable `initialized` in `Scheduler.h`realisiert werden. `initialized` sollte erst auf `true` gesetzt werden, wenn der erste Thread läuft.

In folgender Datei muss Code implementiert werden: `kernel/threads/Scheduler.cc`.


## A5.4: Testanwendung mit Multithreading
Testen Sie das präemptive Multitasking indem Sie eine kleine Demo-Anwendung schreiben in der ein Zähler-Thread läuft, welcher einen Zähler inkrementiert und an einer festen Position auf dem Bildschirm ausgibt. Zusätzlich soll noch ein zweiter Thread erzeugt werden der eine Melodie abspielt. Neben diesen beiden Threads soll zusätzlich der Fortschritt der Systemzeit im Interrupt ausgegeben werden, siehe nachstehende Abbildung.


**Beispielausgab des Testprogramms**

![MTHR](https://github.com/mschoett/hhuTOSc/blob/aufgabe-5/img/mthr.jpg)
