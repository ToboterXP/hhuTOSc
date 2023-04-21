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
#ifndef __CGA_include__
#define __CGA_include__

#include "kernel/IOport.h"
#include "lib/Types.h"


// Konstanten fuer die moeglichen Farben im Attribut-Byte.
class CGA_Color {
public:
    const static uint8_t BLACK = 0;
    const static uint8_t BLUE = 1;
    const static uint8_t GREEN = 2;
    const static uint8_t CYAN = 3;
    const static uint8_t RED = 4;
    const static uint8_t MAGENTA = 5;
    const static uint8_t BROWN = 6;
    const static uint8_t LIGHT_GREY = 7;
    const static uint8_t DARK_GREY = 8;
    const static uint8_t LIGHT_BLUE = 9;
    const static uint8_t LIGHT_GREEN = 10;
    const static uint8_t LIGHT_CYAN = 11;
    const static uint8_t LIGHT_RED = 12;
    const static uint8_t LIGHT_MAGENTA = 13;
    const static uint8_t YELLOW = 14;
    const static uint8_t WHITE = 15;
};

enum class CGA_Register {
    CURSOR_LOW = 14,
    CURSOR_HIGH = 15
};

typedef struct _CGA_Char {
    char character;
    uint8_t attribute;
} CGA_Char;

class CGA {

private:
    IOport index_port;      // Auswahl eines Register der Grafikkarte
    IOport data_port;       // Lese-/Schreib-Zugriff auf Register der Grafikk.

    // Copy Konstrutkor unterbinden
    CGA(const CGA &copy);

    int readRegister(uint8_t index);
    void writeRegister(uint8_t index, uint8_t value);


public:
    // Groesse des Bildschirms (25 Zeilen, 80 Spalten)
    enum { ROWS = 25, COLUMNS = 80 };


    CGA_Char (*CGA_DATA)[ROWS][COLUMNS];  // Startadresse des Buldschirmspeichers

    // Konstruktur mit Initialisierung der Ports
    CGA () : index_port (0x3d4), data_port (0x3d5) {
        CGA_DATA = (CGA_Char(*)[ROWS][COLUMNS])0xb8000;
    }

    // Standardzeichenfarbe
    enum { STD_ATTR = CGA_Color::BLACK << 4 | CGA_Color::LIGHT_GREY };



    // Setzen des Cursors in Spalte x und Zeile y.
    void setCursorPos (int x, int y);

    // Abfragen der Cursorpostion
    void getCursorPos (int& x, int& y);

    void printNewline(int n = 1);

    CGA_Char * getCharacter(int x, int y);

    // Anzeige eines Zeichens mit Attribut an einer bestimmten Stelle
    void show (int x, int y, char character, unsigned char attrib = STD_ATTR);

    // Anzeige von 'n' Zeichen ab der aktuellen Cursorposition
    void print (char* string, int n, unsigned char attrib = STD_ATTR);

    // Verschiebt den Bildschirminhalt um eine Zeile nach oben.
    // Neue Zeile am unteren Bildrand mit Leerzeichen fuellen
    void scrollup (unsigned char attrib = STD_ATTR);

    // Lösche den Textbildschirm
    void clear (unsigned char attrib = STD_ATTR);

    void clearLine(int y, unsigned char attrib = STD_ATTR);

    // Hilfsfunktion zur Erzeugung eines Attribut-Bytes
    unsigned char attribute (uint8_t bg, uint8_t fg, bool blink);
};

#endif
