#include <stdint.h>
#include <stddef.h>
#include <limits.h>

#include "config.h"
#include "interface.h"
#include "Noritake_VFD_CUY.h"
#include "../SysTickDriver/SysTickDriver.h"

static bool initialized = false;

void Noritake_VFD_CUY::exitMode() {
    if (this->cmdmode==1)      // User Setup Mode
        CUY_exitUserSetupMode();
    else if (this->cmdmode==2) // Diagnostic serial mode
        CUY_exitDiagnosticMode();
}

void Noritake_VFD_CUY::esc(uint8_t cmd) {
    command(0x1b);
    command(cmd);
}
void Noritake_VFD_CUY::esc2(uint8_t cmd, uint8_t b) {
    esc(cmd);
    command(b);
}
void Noritake_VFD_CUY::us(uint8_t grp, uint8_t cmd) {
    command(0x1f);
    command(0x28);
    command(grp);
    command(cmd);
}

void Noritake_VFD_CUY::command(uint8_t c) {
    exitMode();
    writePort(c);
}

void Noritake_VFD_CUY::CUY_init() {
    if (!initialized) {
        _delay_ms(NORITAKE_VFD_RESET_DELAY);
        initialized = true;
    }
    initialState();
    initPort();
    esc(0x40);
}

void Noritake_VFD_CUY::CUY_back() {
    command(0x08);
}

void Noritake_VFD_CUY::CUY_forward() {
    command(0x09);
}

void Noritake_VFD_CUY::CUY_lineFeed() {
    command(0x0a);
}

void Noritake_VFD_CUY::CUY_clearScreen() {
    command(0x0c);
}

void Noritake_VFD_CUY::CUY_home() {
    command(0x0b);
}

void Noritake_VFD_CUY::CUY_carriageReturn() {
    command(0x0d);
}

void Noritake_VFD_CUY::CUY_setCursorStyle(CursorStyle style) {
    command(this->cursorStyle = style);
}

void Noritake_VFD_CUY::CUY_useCustomChars(bool enable) {
    esc(0x25);
    command(this->customCharsEnabled = enable);
}

static inline uint8_t reverse(uint8_t data) {
    static const uint8_t nybble[16] PROGMEM = { 0, 0x8, 0x4, 0xc, 0x2, 0xa, 0x6, 0xe, 0x1,
        0x9, 0x5, 0xd, 0x3, 0xb, 0x7, 0xf };
    return (pgm_read_byte(nybble + (data & 15)) << 4) +
        pgm_read_byte(nybble + (data >> 4));
}

void Noritake_VFD_CUY::CUY_defineCustomChar(uint8_t code, FontFormat format, const uint8_t *data) {    
    if (format==CUUFormat || format==LCDFormat) {
        uint8_t s[8], d[5];
        for (uint8_t i = 0; i < 8; i++)
            s[i] = reverse(data[i]) >> 3;       
        d[0] = s[0] + (s[1]<<5);
        d[1] = (s[1]>>3) + (s[2]<<2) + (s[3]<<7);
        d[2] = (s[3]>>1) + (s[4]<<4);
        d[3] = (s[4]>>4) + (s[5]<<1) + (s[6]<<6);
        d[4] = (s[6]>>2) + (s[7]<<3);
        if (format==CUUFormat) // Toggle underline
            d[4] = (s[7] & 1)? (d[4] | 0xf8): (d[4] & ~0xf8);
        data = d;
    }

    esc(0x26);
    command(0x01);
    command(code);
    command(code);
    command(5);
    for (uint8_t i = 0; i < 5; i++)
        command(data[i]);
}

void Noritake_VFD_CUY::CUY_deleteCustomChar(uint8_t code) {
    esc2(0x3f, 0x01);
    command(code);
}


void Noritake_VFD_CUY::CUY_setCharStyle(uint8_t style) {
    esc((style & BlinkingStyle)? 0x42: 0x41);
    esc((style & UnderlineStyle)? 0x55: 0x57);
    this->charStyle = style;
}

void Noritake_VFD_CUY::CUY_setDisplayPriority(Priority mode) {
    this->displayPriority = mode;
    esc(mode==CommandPriority? 0x45: 0x53);
}

void Noritake_VFD_CUY::CUY_setAsciiVariant(AsciiVariant code) {
    esc2(0x52, this->asciiVariant = code);
}

void Noritake_VFD_CUY::CUY_setBlinkSpeed(uint8_t speed) {
    esc(0x54);
    command(this->blinkSpeed = speed);
}

void Noritake_VFD_CUY::CUY_insertSpace() {
    esc2(0x58, 0xf3);
}

void Noritake_VFD_CUY::CUY_deleteChar() {
    esc2(0x58, 0xf4);
}

void Noritake_VFD_CUY::CUY_insertLine() {
    esc2(0x58, 0xf5);
}

void Noritake_VFD_CUY::CUY_deleteLine() {
    esc2(0x58, 0xf6);
}

void Noritake_VFD_CUY::CUY_reset(bool hardwareReset) {
    if (!initialized) {
        _delay_ms(NORITAKE_VFD_RESET_DELAY);
        initialized = true;
    }

    if (hardwareReset)
        hardReset();
    else
        esc2(0x58, 0xff);
    initialState();
}

void Noritake_VFD_CUY::CUY_setCharset(Charset code) {
    esc2(0x74, this->charset = code);
}

void Noritake_VFD_CUY::CUY_setScrollMode(ScrollMode mode) {
    command(0x1f);
    command(this->scrollMode = mode);
}

void Noritake_VFD_CUY::CUY_setCursor(uint8_t col, uint8_t line) {
    if (NORITAKE_VFD_COLS <= col || NORITAKE_VFD_LINES <= line)
        return;
    command(0x1f);
    command(0x24);
    command(col);
    command((uint8_t) 0);
    command(line);
    command((uint8_t) 0);
}

void Noritake_VFD_CUY::CUY_wait(uint8_t time) {
    us(0x61, 0x01);
    command(time);
}

void Noritake_VFD_CUY::CUY_blinkScreen(bool enable, uint8_t onTime, uint8_t offTime, uint8_t times) {
    us(0x61, 0x11);
    command(enable);
    command(onTime);
    command(offTime);
    command(times);
    CUY_setBlinkSpeed(this->blinkSpeed);
}

void Noritake_VFD_CUY::CUY_blinkScreen(bool enable) {
    if (!enable) {
        us(0x61, 0x11);
        command(enable);
        command(0);
        command(0);
        command(0);
        CUY_setBlinkSpeed(this->blinkSpeed);
    }
}

void Noritake_VFD_CUY::CUY_screenSaver(ScreenSaver mode) {
    us(0x61, 0x40);
    command(this->screenSaver = mode);
}

void Noritake_VFD_CUY::CUY_displayOn() {
    us(0x61, 0x40);
    command(this->displayEnabled = true);
}

void Noritake_VFD_CUY::CUY_displayOff() {
    us(0x61, 0x40);
    command(this->displayEnabled = false);
}

void Noritake_VFD_CUY::CUY_setFontStyle(uint8_t style) {
    if ((style & WideFont) && !(style & TallFont)) // no wide font
        return;

    us(0x67, 0x04);
    command((style & AltDescenderFont)? 0x81: 0x80);

    us(0x67, 0x06);
    command((style & AltLargeFont)? 0x01: 0x00);
    
    us(0x67, 0x40);
    command((style & WideFont)? 2: 1);
    command((style & TallFont)? 2: 1);
    this->fontStyle = style;
}

void Noritake_VFD_CUY::CUY_setCharBrightness(uint8_t level) {
    us(0x67, 0x50);
    command(this->charBrightness = (level * 100 + 134) / 135);
    command(0);
    command(0);
}

void Noritake_VFD_CUY::CUY_setScreenBrightness(int level) {
    command(0x1f);
    command(0x58);
    command(this->screenBrightness = (level + 24) / 25);
}

void Noritake_VFD_CUY::CUY_setHorizScrollSpeed(uint8_t speed) {
    command(0x1f);
    command(0x73);
    command(this->horizScrollSpeed = speed);
}

void Noritake_VFD_CUY::crlf() {
    CUY_carriageReturn();
    CUY_lineFeed();
}

void Noritake_VFD_CUY::print(char c) {
    command(c);
}

void Noritake_VFD_CUY::print(const char *str) {
    while (*str)
        command(*str++);
}

void Noritake_VFD_CUY::print(const char *buffer, size_t size) {
    while (size--)
        print(*buffer++);
}

void Noritake_VFD_CUY::print(long number, uint8_t base) {
    if (number < 0) {
        print('-');
        number = -number;
    }
    print((unsigned long)number, base);
}

void Noritake_VFD_CUY::print(int number, uint8_t base) {
    print((long)number, base);
}

void Noritake_VFD_CUY::print(unsigned long number, uint8_t base) {
    if (number/base)
        print(number/base, base);
    print(number%base + (number%base < 10? '0': 'A'-10));
}

void Noritake_VFD_CUY::print(unsigned number, uint8_t base) {
    print((unsigned long)number, base);
}

void Noritake_VFD_CUY::println(char c) {
    print(c);
    crlf();
}

void Noritake_VFD_CUY::println(const char *str) {
    print(str);
    crlf();
}

void Noritake_VFD_CUY::println(const char *buffer, size_t size) {
    print(buffer, size);
    crlf();
}

void Noritake_VFD_CUY::println(long number, uint8_t base) {
    print(number, base);
    crlf();
}

void Noritake_VFD_CUY::println(int number, uint8_t base) {
    println((long) number, base);
}

void Noritake_VFD_CUY::println(unsigned long number, uint8_t base) {
    print(number, base);
    crlf();
}

void Noritake_VFD_CUY::println(unsigned number, uint8_t base) {
    println((unsigned long) number, base);
}

void Noritake_VFD_CUY::insert(char c) {
    CUY_insertSpace();
    print(c);
}

void Noritake_VFD_CUY::insert(const char *str) {
    while (*str)
        insert(*str++);
}

void Noritake_VFD_CUY::insert(const char *buffer, size_t size) {
    while (size--)
        insert(*buffer++);
}

void Noritake_VFD_CUY::insert(long number, uint8_t base) {
    if (number < 0) {
        insert('-');
        number = -number;
    }
    insert((unsigned long)number, base);
}

void Noritake_VFD_CUY::insert(int number, uint8_t base) {
    insert((long)number, base);
}

void Noritake_VFD_CUY::insert(unsigned long number, uint8_t base) {
    if (number/base)
        insert(number/base, base);
    insert(number%base + (number%base < 10? '0': 'A'-10));
}

void Noritake_VFD_CUY::insert(unsigned number, uint8_t base) {
    insert((unsigned long)number, base);
}

void Noritake_VFD_CUY::CUY_enterUserSetupMode() {
    if (this->cmdmode != 1) {
        exitMode();
        us(0x65, 0x01);
        command('I');
        command('N');
        this->cmdmode = 1;
    }
}

void Noritake_VFD_CUY::CUY_exitUserSetupMode() {
    if (this->cmdmode==1) {
        this->cmdmode = 0;
        us(0x65, 0x02);
        command('O');
        command('U');
        command('T');
        initialState();
    }
}

void Noritake_VFD_CUY::CUY_displayStatus(uint8_t param) {
    if (this->cmdmode != 1)
        return;
    this->cmdmode = 0;
    us(0x65, 0x41);
    command(param);
    this->cmdmode = 1;
}

void Noritake_VFD_CUY::CUY_enterDiagnosticMode() {
    if (this->cmdmode != 2) {
        exitMode();
        enterDiagnosticMode();
        this->cmdmode = 2;
    }
}
void Noritake_VFD_CUY::CUY_exitDiagnosticMode() {
    if (this->cmdmode==2) {
        exitDiagnosticMode();
        this->cmdmode = 0;
    }
}

void Noritake_VFD_CUY::CUY_readDiagnosticInfo(const uint8_t *cmd, size_t cmdSize, uint8_t *output, uint8_t outputSize) {
    if (this->cmdmode==2)
        readDiagnosticInfo(cmd, cmdSize, output, outputSize);
}

void Noritake_VFD_CUY::CUY_defineFROMFont(const uint8_t *bits) {
    CUY_enterUserSetupMode(); this->cmdmode = 0;
    us(0x65, 0x14);
    for (uint8_t i = 0; i<244; i++)
    for (uint8_t b = 0; b<5; b++)
        writePort(bits[i*5 + b]);
    CUY_exitUserSetupMode();
    _delay_ms(200);
    initialState();
}

void Noritake_VFD_CUY::initialState() {
    this->cmdmode = 0;
    this->customCharsEnabled = false;
    this->cursorStyle = UnderlineCursor;
    this->scrollMode = WrappingMode;
    this->displayPriority = CommandPriority;
    this->asciiVariant = 0;
    this->charset = 0;
    this->charStyle = 0;
    this->charBrightness = 100;
    this->screenBrightness = 100;
    this->blinkSpeed = 0x40;
    this->horizScrollSpeed = 0;
    this->fontStyle = AltDescenderFont;
    this->displayEnabled = 1;
    this->screenSaver = 1;
}
