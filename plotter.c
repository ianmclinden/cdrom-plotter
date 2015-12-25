// Project: CD ROM plotter
// Homepage: www.HomoFaciens.de
// Author Norbert Heinz
// Modified by Ian McLinden
// Version: 0.5
// Creation date: 28.09.2015
// Modification date: 22.12.2016
// This program is free software you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation version 3 of the License.
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
// For a copy of the GNU General Public License see http://www.gnu.org/licenses/
// 
// For details see:
// http://www.HomoFaciens.de/technics-machines-plotter-cdrom_en_navion.htm

#include <stdio.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <dirent.h>
#include <math.h>
#include <wiringPi.h>
#include <unistd.h>
#include "consts.h"
#include "app_screen.h"
#include "plotter.h"

//++++++++++++++++++++++++++++++ MakeStepX ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void MakeStepX(int direction, long stepPause) {
	StepX += direction;

	if (StepX > 3) {
		StepX = 0;
	}
	if (StepX < 0) {
		StepX = 3;
	}

//You might have to swap the sequence of steps!!!
//If your motor doesn't rotate as expected, try:
// StepX == 0   StepX == 1   StepX == 2   StepX == 3
// 1 0 0 0      0 0 0 1      0 1 0 0      0 0 1 0
// or:
// StepX == 0   StepX == 1   StepX == 2   StepX == 3
// 1 0 0 0      0 0 1 0      0 1 0 0      0 0 0 1
// or:
// StepX == 0   StepX == 1   StepX == 2   StepX == 3
// 1 0 0 0      0 1 0 0      0 0 1 0      0 0 0 1
// or:
// StepX == 0   StepX == 1   StepX == 2   StepX == 3
// 1 0 0 0      0 1 0 0      0 0 0 1      0 0 1 0

	if (StepX == 0) {
		digitalWrite(X_STEPPER01, 1);
		digitalWrite(X_STEPPER02, 0);
		digitalWrite(X_STEPPER03, 0);
		digitalWrite(X_STEPPER04, 0);
	}
	if (StepX == 1) {
		digitalWrite(X_STEPPER01, 0);
		digitalWrite(X_STEPPER02, 0);
		digitalWrite(X_STEPPER03, 1);
		digitalWrite(X_STEPPER04, 0);
	}
	if (StepX == 2) {
		digitalWrite(X_STEPPER01, 0);
		digitalWrite(X_STEPPER02, 1);
		digitalWrite(X_STEPPER03, 0);
		digitalWrite(X_STEPPER04, 0);
	}
	if (StepX == 3) {
		digitalWrite(X_STEPPER01, 0);
		digitalWrite(X_STEPPER02, 0);
		digitalWrite(X_STEPPER03, 0);
		digitalWrite(X_STEPPER04, 1);
	}

	usleep(stepPause);
}

//++++++++++++++++++++++++++++++ MakeStepY ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void MakeStepY(int direction, long stepPause) {
	StepY += direction;

	if (StepY > 3) {
		StepY = 0;
	}
	if (StepY < 0) {
		StepY = 3;
	}

//You might have to swap the sequence of steps!!!
//If your motor doesn't rotate as expected, try:
// StepY == 0   StepY == 1   StepY == 2   StepY == 3
// 1 0 0 0      0 0 0 1      0 1 0 0      0 0 1 0
// or:
// StepY == 0   StepY == 1   StepY == 2   StepY == 3
// 1 0 0 0      0 0 1 0      0 1 0 0      0 0 0 1
// or:
// StepY == 0   StepY == 1   StepY == 2   StepY == 3
// 1 0 0 0      0 1 0 0      0 0 1 0      0 0 0 1
// or:
// StepY == 0   StepY == 1   StepY == 2   StepY == 3
// 1 0 0 0      0 1 0 0      0 0 0 1      0 0 1 0

	if (StepY == 0) {
		digitalWrite(Y_STEPPER01, 1);
		digitalWrite(Y_STEPPER02, 0);
		digitalWrite(Y_STEPPER03, 0);
		digitalWrite(Y_STEPPER04, 0);
	}
	if (StepY == 1) {
		digitalWrite(Y_STEPPER01, 0);
		digitalWrite(Y_STEPPER02, 0);
		digitalWrite(Y_STEPPER03, 1);
		digitalWrite(Y_STEPPER04, 0);
	}
	if (StepY == 2) {
		digitalWrite(Y_STEPPER01, 0);
		digitalWrite(Y_STEPPER02, 1);
		digitalWrite(Y_STEPPER03, 0);
		digitalWrite(Y_STEPPER04, 0);
	}
	if (StepY == 3) {
		digitalWrite(Y_STEPPER01, 0);
		digitalWrite(Y_STEPPER02, 0);
		digitalWrite(Y_STEPPER03, 0);
		digitalWrite(Y_STEPPER04, 1);
	}
//  printf("StepY\n");
	usleep(stepPause);
}

//++++++++++++++++++++++++++++++++++++++ CalculatePlotter ++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int CalculatePlotter(long moveX, long moveY, long stepPause) {
	char TextLine[1000] = "";
	long tempX = 0, tempY = 0;
	int i = 0;
	unsigned char reverseX = 0, reverseY = 0;

	sprintf(TextLine, "Moving X: %ld, Moving Y: %ld", moveX, moveY);
	MessageText(TextLine, MessageX, MessageY - 2, 0);
//  getch();

	if (moveX == 0) {
		if (moveY > 0) {
			for (i = 0; i < moveY; i++) {
				MakeStepY(-1, stepPause);
			}
		}
		if (moveY < 0) {
			for (i = 0; i < -moveY; i++) {
				MakeStepY(1, stepPause);
			}
		}
	}
	if (moveY == 0) {
		if (moveX > 0) {
			for (i = 0; i < moveX; i++) {
				MakeStepX(1, stepPause);
			}
		}
		if (moveX < 0) {
			for (i = 0; i < -moveX; i++) {
				MakeStepX(-1, stepPause);
			}
		}
	}
	if (moveY != 0 && moveX != 0) {
		if (abs(moveX) > abs(moveY)) {
			while (moveY != 0) {
				tempX = moveX / abs(moveY);
				if (tempX == 0) {
					printf("tempX=%ld, moveX=%ld, moveY=%ld    \n", tempX, moveX, moveY);
				}
				if (tempX > 0) {
					for (i = 0; i < tempX; i++) {
						MakeStepX(1, stepPause);
					}
				}
				if (tempX < 0) {
					for (i = 0; i < -tempX; i++) {
						MakeStepX(-1, stepPause);
					}
				}
				moveX -= tempX;
				if (moveY > 0) {
					MakeStepY(-1, stepPause);
					moveY--;
				}
				if (moveY < 0) {
					MakeStepY(1, stepPause);
					moveY++;
				}
			}
			//move remaining X coordinates
			if (moveX > 0) {
				for (i = 0; i < moveX; i++) {
					MakeStepX(1, stepPause);
				}
			}
			if (moveX < 0) {
				for (i = 0; i < -moveX; i++) {
					MakeStepX(-1, stepPause);
				}
			}
		}//if(abs(moveX) > abs(moveY))
		else {
			while (moveX != 0) {
				tempY = moveY / abs(moveX);
				if (tempY == 0) {
					printf("tempY=%ld, moveX=%ld, moveY=%ld    \n", tempX, moveX, moveY);
				}
				if (tempY > 0) {
					for (i = 0; i < tempY; i++) {
						MakeStepY(-1, stepPause);
					}
				}
				if (tempY < 0) {
					for (i = 0; i < -tempY; i++) {
						MakeStepY(1, stepPause);
					}
				}
				moveY -= tempY;
				if (moveX > 0) {
					MakeStepX(1, stepPause);
					moveX--;
				}
				if (moveX < 0) {
					MakeStepX(-1, stepPause);
					moveX++;
				}
			}
			//move remaining Y koordinates
			if (moveY > 0) {
				for (i = 0; i < moveY; i++) {
					MakeStepY(-1, stepPause);
				}
			}
			if (moveY < 0) {
				for (i = 0; i < -moveY; i++) {
					MakeStepY(1, stepPause);
				}
			}
		}
	}

	sprintf(TextLine, "(Moved X: %ld, Moved Y: %ld)", moveX, moveY);
	MessageText(TextLine, MessageX, MessageY - 2, 0);

	return 0;
}
//-------------------------------------- CalculatePlotter --------------------------------------------------------


// TODO : command line argment files
// TODO : print / plot cancel from any key

int main(int argc, char **argv) {

	int MenuLevel = MENU_MAIN;
	int KeyHit = 0;
	int KeyCode[5];
	char FileInfo[3];
	char FileName[200] = "";
	char FullFileName[200] = "";
	char FileNameOld[200] = "";
	struct winsize terminal;
	double Scale = 1.0;
	double OldScale = 1.0;
	double PicWidth = 0.0;
	double PicHeight = 0.0;
	long MoveLength = 1;
	long OldMoveLength = 200;
	int plotterMode = MODE_PRINT;
	int i;
	int SingleKey = 0;
	long stepPause = 10000;
	long currentPlotX = 0, currentPlotY = 0, currentPlotDown = 0;
	int FileSelected = 0;
	int FileStartRow = 0;
	char *pEnd;
	FILE *PlotFile;
	char TextLine[300];
	long xMin = 1000000, xMax = -1000000;
	long yMin = 1000000, yMax = -1000000;
	long coordinateCount = 0;
	char a;
	int ReadState = 0;
	long xNow = 0, yNow = 0;
	long xNow1 = 0, yNow1 = 0;
	long xNow2 = 0, yNow2 = 0;
	long FileSize;
	long LongTemp;
	long DataOffset;
	long HeaderSize;
	long PictureWidth;
	long PictureHeight;
	int IntTemp;
	int ColorDepth;
	long CompressionType;
	long PictureSize;
	long XPixelPerMeter;
	long YPixelPerMeter;
	long ColorNumber;
	long ColorUsed;
	long StepsPerPixelX = (double) (STEP_MAX_X) / 55.0, StepsPerPixelY = (double) STEP_MAX_Y / 55.0;
	long FillBytes = 0;
	struct timeval StartTime, EndTime;
	long coordinatePlot = 0;
	int stopPlot = 0;
	long JetOffset1 = 40, JetOffset2 = 40;
	int ReverseMode, NewLine;
	long CyanDrops, MagentaDrops, YellowDrops;
	int PixelRed, PixelGreen, PixelBlue;
	int PixelRedNext, PixelGreenNext, PixelBlueNext;
	long PlotStartTime = 0;


	FileInfo[2] = '\0';

	strcpy(FileName, "noFiLE");

	getcwd(PicturePath, 1000);
	strcat(PicturePath, "/pictures");
	printf("PicturePath=>%s<", PicturePath);

	if (wiringPiSetup() == -1) {
		printf("Could not run wiringPiSetup!");
		exit(1);
	}

	softPwmCreate(Z_SERVO, SERVOUP, 200);
	softPwmWrite(Z_SERVO, SERVOUP);
	usleep(500000);
	softPwmWrite(Z_SERVO, 0);

	pinMode(X_STEPPER01, OUTPUT);
	pinMode(X_STEPPER02, OUTPUT);
	pinMode(X_STEPPER03, OUTPUT);
	pinMode(X_STEPPER04, OUTPUT);
	pinMode(X_ENABLE01, OUTPUT);
	pinMode(X_ENABLE02, OUTPUT);

	digitalWrite(X_STEPPER01, 1);
	digitalWrite(X_STEPPER02, 0);
	digitalWrite(X_STEPPER03, 0);
	digitalWrite(X_STEPPER04, 0);

	digitalWrite(X_ENABLE01, 1);
	digitalWrite(X_ENABLE02, 1);

	pinMode(Y_STEPPER01, OUTPUT);
	pinMode(Y_STEPPER02, OUTPUT);
	pinMode(Y_STEPPER03, OUTPUT);
	pinMode(Y_STEPPER04, OUTPUT);
	digitalWrite(Y_STEPPER01, 1);
	digitalWrite(Y_STEPPER02, 0);
	digitalWrite(Y_STEPPER03, 0);
	digitalWrite(Y_STEPPER04, 0);


	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &terminal) < 0) {
		printf("Can't get size of terminal window");
	}
	else {
		MaxRows = terminal.ws_row;
		MaxCols = terminal.ws_col;
		MessageY = MaxRows - 3;
	}

	clrscr(1, MaxRows);
	PrintRow('-', MessageY - 1);
	PrintMenu_Main(FileName, Scale, xMax - xMin, yMax - yMin, MoveLength, plotterMode);


	while (1) {
		clrscr(MessageY - 1, MaxRows - 1);
		PrintRow('-', MessageY - 1);
		MessageText("Waiting for command.", MessageX, MessageY, 0);

		i = 0;
		SingleKey = 1;
		KeyCode[0] = 0;
		KeyCode[1] = 0;
		KeyCode[2] = 0;
		KeyCode[3] = 0;
		KeyCode[4] = 0;
		KeyHit = 0;
		while (kbhit()) {
			KeyHit = getch();
			KeyCode[i] = KeyHit;
			i++;
			if (i == 5) {
				i = 0;
			}
			if (i > 1) {
				SingleKey = 0;
			}
		}
		if (SingleKey == 0) {
			KeyHit = 0;
		}

		if (MenuLevel == MENU_MAIN) {

			// Move X-axis -
			if (KeyCode[0] == 27 && KeyCode[1] == 91 && KeyCode[2] == 68 && KeyCode[3] == 0 && KeyCode[4] == 0) {
				CalculatePlotter(-MoveLength, 0, stepPause);
			}

			// Move X-axis +
			if (KeyCode[0] == 27 && KeyCode[1] == 91 && KeyCode[2] == 67 && KeyCode[3] == 0 && KeyCode[4] == 0) {
				CalculatePlotter(MoveLength, 0, stepPause);
			}

			// Move Y-axis -
			if (KeyCode[0] == 27 && KeyCode[1] == 91 && KeyCode[2] == 66 && KeyCode[3] == 0 && KeyCode[4] == 0) {
				CalculatePlotter(0, -MoveLength, stepPause);
			}

			//Move Y-axis +
			if (KeyCode[0] == 27 && KeyCode[1] == 91 && KeyCode[2] == 65 && KeyCode[3] == 0 && KeyCode[4] == 0) {
				CalculatePlotter(0, MoveLength, stepPause);
			}

			// Move Pen UP
			if ((KeyCode[0] == 27 && KeyCode[1] == 91 && KeyCode[2] == 53 && KeyCode[3] == 126 && KeyCode[4] == 0) ||
			    (KeyHit == 'u')) {
				MessageText("Raising Pen", 1, MessageY - 2, 0);
				softPwmWrite(Z_SERVO, SERVOUP);
				usleep(500000);
				softPwmWrite(Z_SERVO, 0);
				currentPlotDown = 0;
				MessageText("(Pen Raised)", 1, MessageY - 2, 0);
			}

			// Move Pen Down
			if ((KeyCode[0] == 27 && KeyCode[1] == 91 && KeyCode[2] == 54 && KeyCode[3] == 126 && KeyCode[4] == 0) ||
			    (KeyHit == 'd')) {
				MessageText("Lowering Pen", 1, MessageY - 2, 0);
				softPwmWrite(Z_SERVO, SERVODOWN);
				usleep(500000);
				softPwmWrite(Z_SERVO, 0);
				currentPlotDown = 1;
				MessageText("(Pen Lowered)", 1, MessageY - 2, 0);
			}


			// Zero Axes
			if (KeyHit == 48) { // 0
				MessageText("Zeroing X,Y,Z", 1, MessageY - 2, 0);
				softPwmWrite(Z_SERVO, SERVOUP);
				usleep(500000);
				softPwmWrite(Z_SERVO, 0);
				currentPlotDown = 0;

				// TODO : move xy to 0,0
			}


			// Move Key Hit
			if (KeyHit == 'm') {
				switch (MoveLength) {
					case 1:
					case 10:
						MoveLength *= 10;
						break;
					case 100:
					default:
						MoveLength = 1;
						break;

				}
				PrintMenu_Main(FileName, Scale, xMax - xMin, yMax - yMin, MoveLength, plotterMode);
			}

			// File Key Hit
			if (KeyHit == 'f') {
				FileStartRow = 0;
				FileSelected = 0;
				strcpy(FileNameOld, FileName);
				strcpy(FileName, PrintMenu_File(FileStartRow, 0));
				MenuLevel = MENU_FILE;
			}

			// Plot Key Hits
			if (KeyHit == 'p') {

				if (strcmp(FileName, "noFiLE") == 0) {
					ErrorText(KRED "No File loaded" KRESET);
					continue;
				}

				if (strcmp(FileName, "noFiLE") != 0) {
					if ((PlotFile = fopen(FullFileName, "rb")) == NULL) {
						sprintf(TextLine, "Can't open file '%s'!\n", FullFileName);
						strcpy(FileName, "noFiLE");
						ErrorText(TextLine);
						continue;
					}
				}

				MessageText(KRED "> 3 seconds until printer starts ." KRESET, 1, MessageY - 2, 0);
				sleep(1);
				MessageText(KRED "> 2 seconds until printer starts .." KRESET, 1, MessageY - 2, 0);
				sleep(1);
				MessageText(KRED "> 1 seconds until printer starts ..." KRESET, 1, MessageY - 2, 0);
				sleep(1);
				MessageText(KGRN "> Running" KRESET, 1, MessageY - 2, 0);

				if (strcmp(FileName, "noFiLE") != 0) {
					if (plotterMode == MODE_PLOT) {
						MessageText(KGRN "Printing: " KRESET "Press P to PAUSE or ESC to ABORT", MessageX, MessageY, 0);
						xNow1 = -1;
						xNow2 = -1;
						yNow1 = -1;
						yNow2 = -1;
						currentPlotX = 0;
						currentPlotY = 0;
						PlotStartTime = time(0);
						PrintMenu_Plot(FullFileName, coordinateCount, 0, 0, 0, PlotStartTime);
						coordinatePlot = 0;
						stopPlot = 0;
						if (currentPlotDown == 1) {
							softPwmWrite(Z_SERVO, SERVOUP);
							currentPlotDown = 0;
							usleep(500000);
							softPwmWrite(Z_SERVO, 0);
						}

						while (!(feof(PlotFile)) && stopPlot == 0) {

							fread(&a, 1, 1, PlotFile);
							i = 0;
							TextLine[0] = '\0';
							while (a != ' ' && a != '<' && a != '>' && a != '\"' && a != '=' && a != ',' && a != ':') {
								TextLine[i] = a;
								TextLine[i + 1] = '\0';
								i++;
								fread(&a, 1, 1, PlotFile);
							}
							if (a == '<') {//Init
								if (xNow2 > -1 && yNow2 > -1 && (xNow2 != xNow1 || yNow2 != yNow1)) {
									stopPlot = CalculatePlotter(xNow2 - currentPlotX, yNow2 - currentPlotY, stepPause);
									if (currentPlotDown == 0) {
										softPwmWrite(Z_SERVO, SERVODOWN);
										usleep(500000);
										softPwmWrite(Z_SERVO, 0);
										currentPlotDown = 1;
									}
									currentPlotX = xNow2;
									currentPlotY = yNow2;

									stopPlot = CalculatePlotter(xNow1 - currentPlotX, yNow1 - currentPlotY, stepPause);
									currentPlotX = xNow1;
									currentPlotY = yNow1;

									stopPlot = CalculatePlotter(xNow - currentPlotX, yNow - currentPlotY, stepPause);
									currentPlotX = xNow;
									currentPlotY = yNow;
								}
								ReadState = 0;
								xNow1 = -1;
								xNow2 = -1;
								yNow1 = -1;
								yNow2 = -1;
							}
							if (strcmp(TextLine, "path") == 0) {
								if (currentPlotDown == 1) {
									softPwmWrite(Z_SERVO, SERVOUP);
									usleep(500000);
									softPwmWrite(Z_SERVO, 0);
									currentPlotDown = 0;
								}
								ReadState = 1;//path found
							}
							if (ReadState == 1 && strcmp(TextLine, "fill") == 0) {
								ReadState = 2;//fill found
							}
							if (ReadState == 2 && strcmp(TextLine, "none") == 0) {
								ReadState = 3;//none found
							}
							if (ReadState == 2 && strcmp(TextLine, "stroke") == 0) {
								ReadState = 0;//stroke found, fill isn't "none"
							}
							if (ReadState == 3 && strcmp(TextLine, "d") == 0 && a == '=') {
								ReadState = 4;//d= found
							}
							if (ReadState == 4 && strcmp(TextLine, "M") == 0 && a == ' ') {
								ReadState = 5;//M found
							}

							if (ReadState == 6) {//Y value
								yNow = (strtol(TextLine, &pEnd, 10) - yMin) * Scale * StepsPermmY / StepsPermmX;
								ReadState = 7;
							}
							if (ReadState == 5 && a == ',') {//X value
								xNow = ((xMax - strtol(TextLine, &pEnd, 10))) * Scale;
								ReadState = 6;
							}
							if (ReadState == 7) {
								if (xNow2 > -1 && yNow2 > -1 && (xNow2 != xNow1 || yNow2 != yNow1)) {
									stopPlot = CalculatePlotter(xNow2 - currentPlotX, yNow2 - currentPlotY, stepPause);
									if (currentPlotDown == 0) {
										softPwmWrite(Z_SERVO, SERVODOWN);
										usleep(500000);
										softPwmWrite(Z_SERVO, 0);
										currentPlotDown = 1;
									}
									currentPlotX = xNow2;
									currentPlotY = yNow2;
								}
								xNow2 = xNow1;
								yNow2 = yNow1;
								xNow1 = xNow;
								yNow1 = yNow;
								ReadState = 5;
							}
						}//while(!(feof(PlotFile)) && stopPlot == 0){
						fclose(PlotFile);
						if (currentPlotDown == 1) {
							softPwmWrite(Z_SERVO, SERVOUP);
							usleep(500000);
							softPwmWrite(Z_SERVO, 0);
							currentPlotDown = 0;
						}
						PrintMenu_Plot(FullFileName, coordinateCount, coordinatePlot, 0, 0, PlotStartTime);
						CalculatePlotter(-currentPlotX, -currentPlotY, stepPause);
						currentPlotX = 0;
						currentPlotY = 0;
						while (kbhit()) {
							getch();
						}
						MessageText("Finished! Press any key to return to main menu.", MessageX, MessageY, 0);
						getch();
						PrintMenu_Main(FileName, Scale, xMax - xMin, yMax - yMin, MoveLength, plotterMode);
					}//if(plotterMode == MODE_PLOT){


					if (plotterMode == MODE_PRINT) {//bitmap
						fread(&FileInfo, 2, 1, PlotFile);
						fread(&FileSize, 4, 1, PlotFile);
						fread(&LongTemp, 4, 1, PlotFile);
						fread(&DataOffset, 4, 1, PlotFile);
						fread(&HeaderSize, 4, 1, PlotFile);
						fread(&PictureWidth, 4, 1, PlotFile);
						fread(&PictureHeight, 4, 1, PlotFile);
						fread(&IntTemp, 2, 1, PlotFile);
						fread(&ColorDepth, 2, 1, PlotFile);
						fread(&CompressionType, 4, 1, PlotFile);
						fread(&PictureSize, 4, 1, PlotFile);
						fread(&XPixelPerMeter, 4, 1, PlotFile);
						fread(&YPixelPerMeter, 4, 1, PlotFile);
						fread(&ColorNumber, 4, 1, PlotFile);
						fread(&ColorUsed, 4, 1, PlotFile);

						FillBytes = 0;
						while ((PictureWidth * 3 + FillBytes) % 4 != 0) {
							FillBytes++;
						}
						CalculatePlotter(0, StepsPerPixelX * PictureWidth, stepPause);
						fseek(PlotFile, DataOffset, SEEK_SET);
						ReverseMode = 0;
						for (currentPlotY = 0; currentPlotY < PictureHeight; currentPlotY++) {
							NewLine = 0;
							if (ReverseMode == 0) {
								currentPlotX = 0;
							}
							else {
								currentPlotX = PictureWidth - 1;
							}
							while (NewLine == 0) {
								fseek(PlotFile, DataOffset + (currentPlotX * PictureWidth + currentPlotX) * 3 +
								                FillBytes * currentPlotY, SEEK_SET);
								fread(&PixelBlue, 1, 1, PlotFile);
								fread(&PixelGreen, 1, 1, PlotFile);
								fread(&PixelRed, 1, 1, PlotFile);

								if (ReverseMode == 1) {
									fseek(PlotFile, DataOffset + (currentPlotX * PictureWidth + currentPlotX - 1) * 3 +
									                FillBytes * currentPlotY, SEEK_SET);
								}
								fread(&PixelBlueNext, 1, 1, PlotFile);
								fread(&PixelGreenNext, 1, 1, PlotFile);
								fread(&PixelRedNext, 1, 1, PlotFile);

								if (PixelRed < 200 || PixelGreen < 200 || PixelBlue < 200) {
									if (currentPlotDown == 0) {
										softPwmWrite(Z_SERVO, SERVODOWN);
										usleep(500000);
										softPwmWrite(Z_SERVO, 0);
										currentPlotDown = 1;
									}
									if (PixelRedNext > 199 && PixelGreenNext > 199 && PixelBlueNext > 199) {
										if (currentPlotDown == 1) {
											softPwmWrite(Z_SERVO, SERVOUP);
											usleep(500000);
											softPwmWrite(Z_SERVO, 0);
											currentPlotDown = 0;
										}
									}
								}
								else {
									if (currentPlotDown == 1) {
										softPwmWrite(Z_SERVO, SERVOUP);
										usleep(500000);
										softPwmWrite(Z_SERVO, 0);
										currentPlotDown = 0;
									}
								}
								if (ReverseMode == 0) {
									currentPlotX++;
									if (currentPlotX < PictureWidth) {
										CalculatePlotter(StepsPerPixelX, 0, stepPause);//X-Y movement swapped!!!
									}
									else {
										NewLine = 1;
										ReverseMode = 1;
									}
								}
								else {
									currentPlotX--;
									if (currentPlotX > -1) {
										CalculatePlotter(-StepsPerPixelX, 0, stepPause);//X-Y movement swapped!!!
									}
									else {
										NewLine = 1;
										ReverseMode = 0;
									}
								}
							}//while(NewLine == 0){
							if (currentPlotDown == 1) {
								softPwmWrite(Z_SERVO, SERVOUP);
								usleep(500000);
								softPwmWrite(Z_SERVO, 0);
								currentPlotDown = 0;
							}
							CalculatePlotter(0, -StepsPerPixelY, stepPause);
						}//for(currentPlotY = 0; currentPlotY < PictureHeight + JetOffset1 + JetOffset2; currentPlotY++){
						fclose(PlotFile);
						if (currentPlotDown == 1) {
							softPwmWrite(Z_SERVO, SERVOUP);
							usleep(500000);
							softPwmWrite(Z_SERVO, 0);
							currentPlotDown = 0;
						}
						if (ReverseMode == 1) {
							CalculatePlotter(-StepsPerPixelX * PictureWidth, 0, stepPause);
						}
					}//if(plotterMode == MODE_PRINT){
				}//if(strcmp(FileName, "noFiLE") != 0){
			}//if(KeyHit == 'p'){

		}//if(MenuLevel == MENU_MAIN){


		if (MenuLevel == MENU_FILE) {//Select file

			// Down Arrow
			if (KeyCode[0] == 27 && KeyCode[1] == 91 && KeyCode[2] == 66 && KeyCode[3] == 0 && KeyCode[4] == 0) {
				FileSelected++;
				strcpy(FileName, PrintMenu_File(FileStartRow, FileSelected));
			}

			// Up Arrow
			if (KeyCode[0] == 27 && KeyCode[1] == 91 && KeyCode[2] == 65 && KeyCode[3] == 0 && KeyCode[4] == 0) {
				if (FileSelected > 0) {
					FileSelected--;
					strcpy(FileName, PrintMenu_File(FileStartRow, FileSelected));
				}
			}

			// Enter
			if (KeyHit == 10) {//Read file and store values
				MenuLevel = MENU_MAIN;
				clrscr(MessageY + 1, MessageY + 1);
				strcpy(FullFileName, PicturePath);
				strcat(FullFileName, "/");
				strcat(FullFileName, FileName);
				if ((PlotFile = fopen(FullFileName, "rb")) == NULL) {
					sprintf(TextLine, "Can't open file '%s'!\n", FullFileName);
					ErrorText(TextLine);
					strcpy(FileName, "noFiLE");
				}
				else {
					if (memcmp(FileName + strlen(FileName) - 4, ".svg", 4) == 0) {
						plotterMode = MODE_PLOT;
					}
					else {
						plotterMode = MODE_PRINT;
					}
					xMin = 1000000;
					xMax = -1000000;
					yMin = 1000000;
					yMax = -1000000;
					coordinateCount = 0;

					if (plotterMode == MODE_PLOT) {

						while (!(feof(PlotFile)) && stopPlot == 0) {

							fread(&a, 1, 1, PlotFile);
							i = 0;
							TextLine[0] = '\0';
							while (a != ' ' && a != '<' && a != '>' && a != '\"' && a != '=' && a != ',' && a != ':') {
								TextLine[i] = a;
								TextLine[i + 1] = '\0';
								i++;
								fread(&a, 1, 1, PlotFile);
							}
							if (a == '<') {//Init
								ReadState = 0;
							}
							if (strcmp(TextLine, "path") == 0) {
								ReadState = 1;//path found
							}
							if (ReadState == 1 && strcmp(TextLine, "fill") == 0) {
								ReadState = 2;//fill found
							}
							if (ReadState == 2 && strcmp(TextLine, "none") == 0) {
								ReadState = 3;//none found
							}
							if (ReadState == 2 && strcmp(TextLine, "stroke") == 0) {
								ReadState = 0;//stroke found, fill isn't "none"
							}
							if (ReadState == 3 && strcmp(TextLine, "d") == 0 && a == '=') {
								ReadState = 4;//d= found
							}
							if (ReadState == 4 && strcmp(TextLine, "M") == 0 && a == ' ') {
								ReadState = 5;//M found
							}

							if (ReadState == 5 && strcmp(TextLine, "C") == 0 && a == ' ') {
								ReadState = 5;//C found
							}

							if (ReadState == 6) {//Y value
								yNow = strtol(TextLine, &pEnd, 10);
								//printf("String='%s' y=%ld\n", TextLine, yNow);
								if (yNow > yMax) {
									yMax = yNow;
								}
								if (yNow < yMin) {
									yMin = yNow;
								}
								ReadState = 7;
							}
							if (ReadState == 5 && a == ',') {//X value
								xNow = strtol(TextLine, &pEnd, 10);
								if (xNow > xMax) {
									xMax = xNow;
								}
								if (xNow < xMin) {
									xMin = xNow;
								}
								ReadState = 6;
							}
							if (ReadState == 7) {
								//printf("Found koordinates %ld, %ld\n", xNow, yNow);
								ReadState = 5;
							}
							gotoxy(1, MessageY);
							printf("ReadState=% 3d, xNow=% 10ld, xMin=% 10ld, xMax=% 10ld, yMin=% 10ld, yMax=% 10ld   ",
							       ReadState, xNow, xMin, xMax, yMin, yMax);

						}//while(!(feof(PlotFile)) && stopPlot == 0){
						fclose(PlotFile);
						if (xMax - xMin > yMax - yMin) {
							Scale = STEP_MAX_X / (double) (xMax - xMin);
						}
						else {
							Scale = STEP_MAX_X / (double) (yMax - yMin);
						}
						//getch();
					}//if(plotterMode == MODE_PLOT){

					if (plotterMode == MODE_PRINT) {//bitmap
						// TODO : error catch here
						fread(&FileInfo, 2, 1, PlotFile);
						fread(&FileSize, 4, 1, PlotFile);
						fread(&LongTemp, 4, 1, PlotFile);
						fread(&DataOffset, 4, 1, PlotFile);
						fread(&HeaderSize, 4, 1, PlotFile);
						fread(&PictureWidth, 4, 1, PlotFile);
						fread(&PictureHeight, 4, 1, PlotFile);
						fread(&IntTemp, 2, 1, PlotFile);
						fread(&ColorDepth, 2, 1, PlotFile);
						fread(&CompressionType, 4, 1, PlotFile);
						fread(&PictureSize, 4, 1, PlotFile);
						fread(&XPixelPerMeter, 4, 1, PlotFile);
						fread(&YPixelPerMeter, 4, 1, PlotFile);
						fread(&ColorNumber, 4, 1, PlotFile);
						fread(&ColorUsed, 4, 1, PlotFile);
						if (FileInfo[0] != 'B' || FileInfo[1] != 'M') {
							sprintf(TextLine, "Wrong Fileinfo: %s (BM)!\n", FileInfo);
							ErrorText(TextLine);
							strcpy(FileName, "noFiLE");
						}
						if (PictureWidth != 55 || PictureHeight != 55) {
							sprintf(TextLine, "Wrong file size (must be 55 x 55): %ld x %ld!\n", PictureWidth,
							        PictureHeight);
							ErrorText(TextLine);
							strcpy(FileName, "noFiLE");
						}
						if (ColorDepth != 24) {
							sprintf(TextLine, "Wrong ColorDepth: %d (must be 24)!\n", ColorDepth);
							ErrorText(TextLine);
							strcpy(FileName, "noFiLE");
						}
						if (CompressionType != 0) {
							sprintf(TextLine, "Wrong CompressionType: %ld (0)!\n", CompressionType);
							ErrorText(TextLine);
							strcpy(FileName, "noFiLE");
						}
						xMin = 0;
						xMax = PictureWidth * StepsPerPixelX;
						yMin = 0;
						yMax = PictureHeight * StepsPerPixelY;
						coordinateCount = PictureWidth * PictureHeight;
						Scale = 1.0;
					}
				}
				//PicWidth = (double)(xMax - xMin) * Scale;
				//PicHeight = (double)(yMax - yMin) * Scale;
				PrintMenu_Main(FileName, Scale, xMax - xMin, yMax - yMin, MoveLength, plotterMode);
			}//if(KeyHit == 10){

		}//if(MenuLevel == MENU_FILE){

		// Escape
		if (KeyHit == 27) {
			if (MenuLevel == MENU_MAIN) {
				clrscr(MessageY + 1, MessageY + 1);
				MessageText("Exit program (y/n)?", MessageX, MessageY + 1, 0);
				while (KeyHit != 'y' && KeyHit != 'n') {
					KeyHit = getch();
					if (KeyHit == 'y') {
						digitalWrite(X_STEPPER01, 0);
						digitalWrite(X_STEPPER02, 0);
						digitalWrite(X_STEPPER03, 0);
						digitalWrite(X_STEPPER04, 0);
						digitalWrite(X_ENABLE01, 0);
						digitalWrite(X_ENABLE02, 0);

						digitalWrite(Y_STEPPER01, 0);
						digitalWrite(Y_STEPPER02, 0);
						digitalWrite(Y_STEPPER03, 0);
						digitalWrite(Y_STEPPER04, 0);
						exit(0);
					}
				}
			}
			if (MenuLevel == MENU_FILE) {
				MenuLevel = MENU_MAIN;
				strcpy(FileName, FileNameOld);
				PrintMenu_Main(FileName, Scale, xMax - xMin, yMax - yMin, MoveLength, plotterMode);
			}
			clrscr(MessageY + 1, MessageY + 1);
		}
	}

	return 0;
}
