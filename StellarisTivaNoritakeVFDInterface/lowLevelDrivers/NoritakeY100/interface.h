#if 0
#define DIRECTION(X,D)	if (D) (*(&X##_PORT - 1) |= (1<<X##_PIN)); else (*(&X##_PORT - 1) &= ~(1<<X##_PIN))
#define RAISE(X)		X##_PORT |= (1<<X##_PIN)
#define LOWER(X)		X##_PORT &= ~(1<<X##_PIN)
#define CHECK(X)		(*(&X##_PORT - 2) & (1<<X##_PIN))
#endif

#if 1
#define RAISE(X,Y)	HWREG(X + GPIO_O_DATA + (Y << 2)) = Y;
#define LOWER(X,Y)	HWREG(X + GPIO_O_DATA + (Y << 2)) = 0;
#define CHECK(X,Y)	(HWREG(X + (GPIO_O_DATA + (Y << 2))))
#endif


void initPort();
void writePort(uint8_t data);
void enterDiagnosticMode();
void exitDiagnosticMode();
void readDiagnosticInfo(const uint8_t *cmd, size_t cmdSize, uint8_t *output, uint8_t outputSize);
void hardReset();


