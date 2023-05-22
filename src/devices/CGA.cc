/*****************************************************************************
 *                                                                           *
 *                                    C G A                                  *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Mit Hilfe dieser Klasse kann man auf den Bildschirm des  *
 *                  PCs zugreifen. Der Zugriff erfolgt direkt auf der Hard-  *
 *                  wareebene, d.h. ueber den Bildschirmspeicher und den     *
 *                  I/O-Ports der Grafikkarte.                               *
 *                                                                           *
 * Autor:           Olaf Spinczyk, TU Dortmund                               *
 *                  Aenderungen von Michael Schoettner, HHU, 21.8.2016       *
 *****************************************************************************/
#include "devices/CGA.h"
#include "kernel/Globals.h"


int CGA::readRegister(uint8_t index) {
    index_port.outb(index);
    return data_port.inb();
}


void CGA::writeRegister(uint8_t index, uint8_t value) {
    index_port.outb(index);
    data_port.outb(value);
}

/*bool CGA::HasGraphicsLock() {
    return !scheduler.get_active() || graphics_lock.has_acquired(scheduler.get_active());
}*/


void CGA::AcquireGraphicsLock() {
    graphics_lock.acquire();
}

void CGA::ReleaseGraphicsLock() {
    graphics_lock.release();
}


/*****************************************************************************
 * Methode:         CGA::setCursorPos                                              *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Setzen des Cursors in Spalte x und Zeile y.              *
 *****************************************************************************/
void CGA::setCursorPos (int x, int y) {

    if (x < 0 || x >= COLUMNS || y < 0 || y >= ROWS) return;
    int offset = x + y * COLUMNS;

    writeRegister(14, (offset & 0xff00)>>8 );
    writeRegister(15, offset & 0xff);
}


/*****************************************************************************
 * Methode:         CGA::getCursorPos                                              *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Abfragem der Cursorposition                              *
 *                                                                           *
 * Rückgabewerte:   x und y                                                  *
 *****************************************************************************/
void CGA::getCursorPos (int &x, int &y) {

    int high = readRegister(14);
    int low = readRegister(15);
    int offset = (high<<8) | low;

    x = offset % COLUMNS;
    y = offset / COLUMNS;
}


/*****************************************************************************
 * Methode:         CGA::show                                                *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Anzeige eines Zeichens mit Attribut an einer bestimmten  *
 *                  Stelle auf dem Bildschirm.                               *
 *                                                                           *
 * Parameter:                                                                *
 *      x,y         Position des Zeichens                                    *
 *      character   Das auszugebende Zeichen                                 *
 *      attrib      Attributbyte fuer das Zeichen                            *
 *****************************************************************************/
 CGA_Char * CGA::getCharacter(int x, int y) {
     return &((*CGA_DATA)[y][x]);
 }

void CGA::show (int x, int y, char character, unsigned char attrib) {
    (*CGA_DATA)[y][x].character = character;
    (*CGA_DATA)[y][x].attribute = attrib;
}


void CGA::printNewline(int n) {

    int cursorX, cursorY;
    getCursorPos(cursorX, cursorY);

    cursorX = 0;
    cursorY += n;
    while (cursorY >= ROWS) {
        scrollup();
        cursorY--;
    }

    setCursorPos(cursorX, cursorY);
}


/*****************************************************************************
 * Methode:         CGA::print                                               *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Anzeige von 'n' Zeichen ab der aktuellen Cursorposition  *
 *                  '\n' fuer Zeilenvorschub.                                *
 *                                                                           *
 * Parameter:                                                                *
 *      string      Auszugebende Zeichenkette                                *
 *      n           Laenger der Zeichenkette                                 *
 *      attrib      Attributbyte fuer alle Zeichen der Zeichenkette          *
 *****************************************************************************/
void CGA::print (char* string, int n, unsigned char attrib) {
    int cursorX, cursorY;

    

    for (int i=0; i<n; i++) {
        if (string[i] == '\n') printNewline();
        if (string[i] < 0x20) continue; //ignore control chars

        getCursorPos(cursorX, cursorY);
        show(cursorX, cursorY, string[i], attrib);
        cursorX++;

        if (cursorX >= COLUMNS) printNewline();
        else setCursorPos(cursorX, cursorY);
    }
}


/*****************************************************************************
 * Methode:         CGA::scrollup                                            *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Verschiebt den Bildschirminhalt um eine Zeile nach oben. *
 *                  Die neue Zeile am unteren Bildrand wird mit Leerzeichen  *
 *                  gefuellt.                                                *
 *****************************************************************************/
void CGA::scrollup (unsigned char attrib) {
    for (int x = 0; x < COLUMNS; x++) {
        for (int y = 0; y < ROWS-1; y++) {
            (*CGA_DATA)[y][x] = (*CGA_DATA)[y+1][x];
        }
    }

    for (int x = 0; x < COLUMNS; x++) {
        show(x, ROWS-1, ' ', attrib);
    }
}


/*****************************************************************************
 * Methode:         CGA::clear                                               *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Lösche den Textbildschirm.                               *
 *****************************************************************************/
void CGA::clear (unsigned char attrib) {

    for (int x = 0; x < COLUMNS; x++) {
        for (int y = 0; y < ROWS; y++) {
            (*CGA_DATA)[y][x].character = ' ';
            (*CGA_DATA)[y][x].attribute = attrib;
        }
    }

    setCursorPos(0,0);
}

void CGA::clearLine(int y, unsigned char attrib) {
    for (int x = 0; x < COLUMNS; x++) {
        (*CGA_DATA)[y][x].character = ' ';
        (*CGA_DATA)[y][x].attribute = attrib;
    }
}


/*****************************************************************************
 * Methode:         CGA::attribute                                           *
 *---------------------------------------------------------------------------*
 * Beschreibung:    Hilfsfunktion zur Erzeugung eines Attribut-Bytes aus     *
 *                  Hintergrund- und Vordergrundfarbe und der Angabe, ob das *
 *                  Zeichen blinkend darzustellen ist.                       *
 *                                                                           *
 * Parameter:                                                                *
 *      bg          Background color                                         *
 *      fg          Foreground color                                         *
 *      blink       ywa/no                                                   *
 *****************************************************************************/
unsigned char CGA::attribute (uint8_t bg, uint8_t fg, bool blink) {

    return fg | ((bg & 0b111) << 4) | ((blink ? 1:0) << 7);
}
