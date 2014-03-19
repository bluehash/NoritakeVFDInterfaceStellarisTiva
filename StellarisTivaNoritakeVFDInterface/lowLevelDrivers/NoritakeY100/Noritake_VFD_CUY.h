#include <stdint.h>
#include <stddef.h>

enum CursorStyle {
    UnderlineCursor =         0x13,
    NoCursor =                0x14,
    BlockCursor =             0x15,
    BlinkingUnderlineCursor = 0x16
};

enum CharStyle {
    BlinkingStyle =    0x01,
    UnderlineStyle =   0x02,
    NoStyle =          0x00
};

enum Priority {
    CommandPriority =  0,
    DisplayPriority =  1
};

enum ScrollMode {
    WrappingMode =    1,
    VertScrollMode =  2,
    HorizScrollMode = 3
};

enum ScreenSaver {
    AllDotsOffSaver  = 2,
    AllDotsOnSaver   = 3
};

enum FontStyle {
    AltDescenderFont =   0x01,
    NoAltDescenderFont = 0x00,
    AltLargeFont =       0x02,
    NoAltLargeFont =     0x00,
    TallFont =           0x04,
    WideFont =           0x08,
    NoFontStyle =        0x00
};

enum FontFormat {
    CUYFormat = 0,
    CUUFormat = 1,
    LCDFormat = 2
};

enum AsciiVariant {
    AmericaAscii =  0,
    FranceAscii =   1,
    GermanyAscii =  2,
    EnglandAscii =  3,
    Denmark1Ascii = 4,
    SweedenAscii =  5,
    ItalyAscii =    6,
    Spain1Ascii =   7,
    JapanAscii =    8,
    NorwayAscii =   9,
    Denmark2Ascii = 10,
    Spain2Ascii =   11,
    LatinAmericaAscii = 12,
    KoreaAscii = 13
};

enum StatusCode {
    FirmwareVersionStatusCode = 0x02,
    FirmwareChecksumStatusCode = 0x20,
    FROMChecksumStatusCode = 0x30
};

enum Charset {
    CP437 = 0, EuroStdCharset = CP437,
    Katakana = 1,
    CP850 = 2, MultilingualCharset = CP850,
    CP860 = 3, PortugeseCharset = CP860,
    CP863 = 4, CanadianFrenchCharset = CP863,
    CP865 = 5, NordicCharset = CP865,
    CP1252 = 0x10,
    CP866 = 0x11, Cyrillic2Charset = CP866,
    CP852 = 0x12, Latin2Charset = CP852,
    CP858 = 0x13,
    ROMCharset = 0xff
};

class Noritake_VFD_CUY {
    public:

    uint8_t         cmdmode;
    bool            customCharsEnabled;
    uint8_t         scrollMode;
    uint8_t         displayPriority;
    uint8_t         asciiVariant;
    uint8_t         charset;
    uint8_t         charStyle;
    uint8_t         blinkSpeed;
    uint8_t         horizScrollSpeed;
    uint8_t         fontStyle;
    uint8_t         cursorStyle;
    bool            displayEnabled;
    uint8_t         screenSaver;
    int             screenBrightness;
    int             charBrightness;
    
    void CUY_back();
    void CUY_forward();
    void CUY_lineFeed();
    void CUY_home();
    void CUY_clearScreen();
    void CUY_carriageReturn();
    void CUY_setCursorStyle(CursorStyle style);
    void CUY_useCustomChars(bool enable);
    void CUY_defineCustomChar(uint8_t code, FontFormat format, const uint8_t *data);
    void CUY_deleteCustomChar(uint8_t code);
    void CUY_init();
    void CUY_setCharStyle(uint8_t style);
    void CUY_setAsciiVariant(AsciiVariant code);
    void CUY_setDisplayPriority(Priority mode);
    void CUY_setBlinkSpeed(uint8_t speed);
    void CUY_insertSpace();
    void CUY_deleteChar();
    void CUY_insertLine();
    void CUY_deleteLine();
    void CUY_reset(bool hardwareReset);
    void CUY_setCharset(Charset code);
    void CUY_setScrollMode(ScrollMode mode);
    void CUY_setCursor(uint8_t col, uint8_t line);
    void CUY_wait(uint8_t time);
    void CUY_blinkScreen(bool enable, uint8_t onTime, uint8_t offTime, uint8_t times);
    void CUY_blinkScreen(bool enable);
    void CUY_screenSaver(ScreenSaver mode);
    void CUY_displayOn();
    void CUY_displayOff();
    void CUY_enterUserSetupMode();
    void CUY_defineFROMFont(const uint8_t *bits);
    void CUY_exitUserSetupMode();
    void CUY_readDiagnosticInfo(const uint8_t *cmd, size_t cmdSize, uint8_t *output, uint8_t outputSize);
    void CUY_displayStatus(uint8_t param);
    void CUY_enterDiagnosticMode();
    void CUY_exitDiagnosticMode();
    void CUY_setFontStyle(uint8_t style);
    void CUY_setCharBrightness(uint8_t level);
    void CUY_setScreenBrightness(int level);
    void CUY_setHorizScrollSpeed(uint8_t time);
    void crlf();
    void print(char c);
    void print(const char *str);
    void print(const char *buffer, size_t size);
    void print(long number, uint8_t base);
    void print(int number, uint8_t base);
    void print(unsigned long number, uint8_t base);
    void print(unsigned number, uint8_t base);
    void println(char c);
    void println(const char *str);
    void println(const char *buffer, size_t size);
    void println(long number, uint8_t base);
    void println(int number, uint8_t base);
    void println(unsigned long number, uint8_t base);   
    void println(unsigned number, uint8_t base);
    void insert(char c);
    void insert(const char *str);
    void insert(const char *buffer, size_t size);
    void insert(long number, uint8_t base);
    void insert(int number, uint8_t base);
    void insert(unsigned long number, uint8_t base);
    void insert(unsigned number, uint8_t base);

    protected:

    void command(uint8_t c);
    void esc(uint8_t cmd);
    void esc2(uint8_t cmd, uint8_t b);
    void us(uint8_t grp, uint8_t cmd);
    void exitMode();
    void initialState();
};
