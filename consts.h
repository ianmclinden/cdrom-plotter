#ifndef CDROM_PLOTTER_CONSTS_H
#define CDROM_PLOTTER_CONSTS_H

/* ---- Color Constants ----------------------------------------------------------------------------------------------*/

#define KRESET  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define BRED  "\x1B[1;31m"
#define BGRN  "\x1B[1;32m"
#define BYEL  "\x1B[1;33m"
#define BBLU  "\x1B[1;34m"
#define BMAG  "\x1B[1;35m"
#define BCYN  "\x1B[1;36m"
#define BWHT  "\x1B[1;37m"

#define URED  "\x1B[4;31m"
#define UGRN  "\x1B[4;32m"
#define UYEL  "\x1B[4;33m"
#define UBLU  "\x1B[4;34m"
#define UMAG  "\x1B[4;35m"
#define UCYN  "\x1B[4;36m"
#define UWHT  "\x1B[4;37m"

/* ---- Menu Level Constants -----------------------------------------------------------------------------------------*/

#define MENU_MAIN   0
#define MENU_FILE   1
#define MENU_PLOT   2

/* ---- Plotter Mode Constants ---------------------------------------------------------------------------------------*/

#define MODE_PRINT  0
#define MODE_PLOT   1

/* ---- Axis Constants -----------------------------------------------------------------------------------------------*/

#define SERVOUP     10
#define SERVODOWN   20

#define X_STEPPER01 13
#define X_STEPPER02 14
#define X_STEPPER03 2
#define X_STEPPER04 3

#define X_ENABLE01  12
#define X_ENABLE02  0

#define Y_STEPPER01 6
#define Y_STEPPER02 10
#define Y_STEPPER03 7
#define Y_STEPPER04 9

#define Y_ENABLE01  6
#define y_ENABLE02  15

#define STEP_MAX_X  220.0
#define STEP_MAX_Y  220.0

#define Z_SERVO     8

#define BUFFERSIZE  120

/* ---- Globals ------------------------------------------------------------------------------------------------------*/

#ifdef  CDROM_PLOTTER_PLOTTER_H
// Terminal GUI Variables
int MaxRows = 24;
int MaxCols = 80;
int MessageX = 1;
int MessageY = 24;
unsigned char MoveBuffer[BUFFERSIZE];

// Stepper Motor Drivers
int StepX = 0;
int StepY = 0;
double StepsPermmX = 250.0 / 35.0;
double StepsPermmY = 250.0 / 35.0;

// Input Filepath
char PicturePath[1000];

#else
// Terminal GUI Variables
extern int MaxRows = 24;
extern int MaxCols = 80;
extern int MessageX = 1;
extern int MessageY = 24;
extern unsigned char MoveBuffer[BUFFERSIZE];

// Stepper Motor Drivers
extern int StepX = 0;
extern int StepY = 0;
extern double StepsPermmX = 250.0 / 35.0;
extern double StepsPermmY = 250.0 / 35.0;

// Input Filepath
extern char PicturePath[1000];

#endif //CDROM_PLOTTER_CONSTS_H
