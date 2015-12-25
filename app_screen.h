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
#include <string.h>
#include <dirent.h>
#include <time.h>
#include <sys/time.h>
#include <wiringPi.h>
#include <softPwm.h>
#include "plotter.h"

//+++++++++++++++++++++++ Start gotoxy ++++++++++++++++++++++++++
//Thanks to 'Stack Overflow', found on http://www.daniweb.com/software-development/c/code/216326
int gotoxy(int x, int y) {
	char essq[100]; // String variable to hold the escape sequence
	char xstr[100]; // Strings to hold the x and y coordinates
	char ystr[100]; // Escape sequences must be built with characters

	//Convert the screen coordinates to strings.
	sprintf(xstr, "%d", x);
	sprintf(ystr, "%d", y);

	//Build the escape sequence (vertical move).
	essq[0] = '\0';
	strcat(essq, "\033[");
	strcat(essq, ystr);

	//Described in man terminfo as vpa=\E[%p1%dd. Vertical position absolute.
	strcat(essq, "d");

	//Horizontal move. Horizontal position absolute
	strcat(essq, "\033[");
	strcat(essq, xstr);
	// Described in man terminfo as hpa=\E[%p1%dG
	strcat(essq, "G");

	//Execute the escape sequence. This will move the cursor to x, y
	printf("%s", essq);
	return 0;
}
//------------------------ End gotoxy ----------------------------------

//+++++++++++++++++++++++ Start clrscr ++++++++++++++++++++++++++
void clrscr(int StartRow, int EndRow) {
	int i, i2;

	if (EndRow < StartRow) {
		i = EndRow;
		EndRow = StartRow;
		StartRow = i;
	}
	gotoxy(1, StartRow);
	for (i = 0; i <= EndRow - StartRow; i++) {
		for (i2 = 0; i2 < MaxCols; i2++) {
			printf(" ");
		}
		printf("\n");
	}
}
//----------------------- End clrscr ----------------------------

//+++++++++++++++++++++++ Start kbhit ++++++++++++++++++++++++++++++++++
//Thanks to Undertech Blog, http://www.undertec.de/blog/2009/05/kbhit_und_getch_fur_linux.html
int kbhit(void) {

	struct termios term, oterm;
	int fd = 0;
	int c = 0;

	tcgetattr(fd, &oterm);
	memcpy(&term, &oterm, sizeof(term));
	term.c_lflag = term.c_lflag & (!ICANON);
	term.c_cc[VMIN] = 0;
	term.c_cc[VTIME] = 1;
	tcsetattr(fd, TCSANOW, &term);
	c = getchar();
	tcsetattr(fd, TCSANOW, &oterm);
	if (c != -1)
		ungetc(c, stdin);

	return ((c != -1) ? 1 : 0);

}
//------------------------ End kbhit -----------------------------------

//+++++++++++++++++++++++ Start getch ++++++++++++++++++++++++++++++++++
//Thanks to Undertech Blog, http://www.undertec.de/blog/2009/05/kbhit_und_getch_fur_linux.html
int getch() {
	static int ch = -1, fd = 0;
	struct termios new_ch, old_ch;

	fd = fileno(stdin);
	tcgetattr(fd, &old_ch);
	new_ch = old_ch;
	new_ch.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(fd, TCSANOW, &new_ch);
	ch = getchar();
	tcsetattr(fd, TCSANOW, &old_ch);

//	printf("ch=%d ", ch);

	return ch;
}
//------------------------ End getch -----------------------------------

//++++++++++++++++++++++ Start MessageText +++++++++++++++++++++++++++++
void MessageText(char *message, int x, int y, int alignment) {
	int i;
	char TextLine[300];

	clrscr(y, y);
	gotoxy(x, y);

	TextLine[0] = '\0';
	if (alignment == 1) {
		for (i = 0; i < (MaxCols - strlen(message)) / 2; i++) {
			strcat(TextLine, " ");
		}
	}
	strcat(TextLine, message);
	printf("%s\n", TextLine);

	gotoxy(1, MessageY + 1);
}
//-------------------------- End MessageText ---------------------------

//++++++++++++++++++++++ Start PrintRow ++++++++++++++++++++++++++++++++
void PrintRow(char character, int y) {
	int i;
	gotoxy(1, y);
	for (i = 0; i < MaxCols; i++) {
		printf("%c", character);
	}
	gotoxy(1, MessageY + 1);
}
//-------------------------- End PrintRow ------------------------------

//+++++++++++++++++++++++++ ErrorText +++++++++++++++++++++++++++++
void ErrorText(char *message) {
	clrscr(MessageY + 2, MessageY - 2);
	gotoxy(1, MessageY - 2);
	printf(BRED
			       "Error: %s"
			       KRESET, message);
	gotoxy(1, MessageY + 1);
}
//----------------------------- ErrorText ---------------------------

//+++++++++++++++++++++++++ PrintMenu_Main ++++++++++++++++++++++++++++++
void PrintMenu_Main(char *PlotFile, double scale, double width, double height, long MoveLength, int plotterMode) {
	char TextLine[300];

	clrscr(1, MessageY - 2);
	MessageText(UWHT "-- CD ROM  Plotter v0.5 --" KRESET, 1, 1, 1);

	sprintf(TextLine, "M            - Toggle move length, current value = %ld step(s)", MoveLength);
	MessageText(TextLine, 10, 4, 0);
	MessageText("<Up>         - Move plotter in Y+ direction", 10, 5, 0);
	MessageText("<Down>       - Move plotter in Y- direction", 10, 6, 0);
	MessageText("<Left>       - Move plotter in X- direction", 10, 7, 0);
	MessageText("<Right>      - Move plotter in X+ direction", 10, 8, 0);
	MessageText("<PgUp> / U   - Lift pen", 10, 8, 0);
	MessageText("<PgDn> / D   - Lower pen", 10, 10, 0);
	MessageText("F            - Choose Print file", 10, 11, 0);
	MessageText("0            - Move plotter to (X,Y,Z) = (0,0,Up)", 10, 12, 0);
	MessageText("P            - Print/Plot file", 10, 13, 0);

	MessageText("Esc          - Exit program", 10, 15, 0);

	// Plotter Status
	MessageText("##################################################################", 1, 17, 1);
	MessageText("#                       Plotter Information:                     #", 1, 18, 1);

	if (strncmp(PlotFile, "noFiLE", strlen(PlotFile)) == 0) {
		MessageText(BRED "No File Loaded" KRESET, 16, 20, 0);
		MessageText(KRED "Cannot Display file information..." KRESET, 20, 21, 0);
		MessageText("", 16, 22, 0);
		MessageText("", 16, 23, 0);
		MessageText("", 16, 24, 0);
	} else {
		sprintf(TextLine, UWHT "File = \"%s\"" KRESET, PlotFile);
		MessageText(TextLine, 16, 20, 0);
		sprintf(TextLine, "Scale       = %0.4f", scale);
		MessageText(TextLine, 20, 21, 0);
		sprintf(TextLine, "Width       = %0.2f cm", (width * scale / 10.0 / StepsPermmX));
		MessageText(TextLine, 20, 22, 0);
		sprintf(TextLine, "Height      = %0.2f cm", (height * scale / 10.0 / StepsPermmX));
		MessageText(TextLine, 20, 23, 0);
		if (plotterMode == MODE_PLOT)
			sprintf(TextLine, "File Mode   = " BRED "PLOT " KRESET "(Vector)");
		if (plotterMode == MODE_PRINT)
			sprintf(TextLine, "File Mode   = " BBLU "PRINT " KRESET "(Raster)");
		MessageText(TextLine, 20, 24, 0);
	}

	MessageText("#                                                                #", 1, 25, 1);
	MessageText("##################################################################", 1, 26, 1);


}
//------------------------- PrintMenu_Main ------------------------------

//+++++++++++++++++++++++++ PrintMenu_File ++++++++++++++++++++++++++++++
char *PrintMenu_File(int StartRow, int selected) {
	char TextLine[300];
	char OpenDirName[1000];
	static char FileName[101];
	DIR *pDIR;
	struct dirent *pDirEnt;
	int i = 0;
	int Discard = 0;

	clrscr(1, MessageY - 2);
	MessageText(UWHT "-- CD ROM  Plotter v0.5 --" KRESET, 1, 1, 1);
	MessageText(BWHT "-- Open File--" KRESET, 1, 2, 1);

	strcpy(OpenDirName, PicturePath);


	pDIR = opendir(OpenDirName);
	if (pDIR == NULL) {
		sprintf(TextLine, "Could not open directory '%s'!", OpenDirName);
		MessageText(TextLine, 1, 5, 1);
		getch();
		return ("noFiLE");
	}

	pDirEnt = readdir(pDIR);
	while (pDirEnt != NULL && i < 10) {
		if (strlen(pDirEnt->d_name) > 4) {
			if (memcmp(pDirEnt->d_name + strlen(pDirEnt->d_name) - 4, ".svg", 4) == 0 ||
			    memcmp(pDirEnt->d_name + strlen(pDirEnt->d_name) - 4, ".bmp", 4) == 0) {
				if (Discard >= StartRow) {
					if (i + StartRow == selected) {
						sprintf(TextLine, ">%s<", pDirEnt->d_name);
						strcpy(FileName, pDirEnt->d_name);
					}
					else {
						sprintf(TextLine, " %s ", pDirEnt->d_name);
					}
					MessageText(TextLine, 1, 4 + i, 0);
					i++;
				}
				Discard++;

			}
		}
		pDirEnt = readdir(pDIR);
	}

	gotoxy(MessageX, MessageY + 1);
	printf("Choose file with <Up>/<Down>. Confirm with 'Enter.' Press 'Esc' to cancel.");


	return (FileName);
}
//------------------------- PrintMenu_File ------------------------------

//+++++++++++++++++++++++++ PrintMenu_Plot ++++++++++++++++++++++++++++++
void PrintMenu_Plot(char *FullFileName, long NumberOfLines, long CurrentLine, long CurrentX, long CurrentY,
                    long StartTime) {
	char TextLine[300];
	long CurrentTime, ProcessHours = 0, ProcessMinutes = 0, ProcessSeconds = 0;

	CurrentTime = time(0);

	CurrentTime -= StartTime;

	while (CurrentTime > 3600) {
		ProcessHours++;
		CurrentTime -= 3600;
	}
	while (CurrentTime > 60) {
		ProcessMinutes++;
		CurrentTime -= 60;
	}
	ProcessSeconds = CurrentTime;

	clrscr(1, MessageY - 2);
	MessageText(UWHT "-- CD ROM  Plotter v0.5 --" KRESET, 1, 1, 1);
	MessageText(BWHT "-- Plotting --" KRESET, 1, 2, 1);


	sprintf(TextLine, "File name: %s", FullFileName);
	MessageText(TextLine, 10, 4, 0);
	sprintf(TextLine, "Number of lines: %ld", NumberOfLines);
	MessageText(TextLine, 10, 5, 0);
	sprintf(TextLine, "Current Position(%ld): X = %ld, Y = %ld     ", CurrentLine, CurrentX, CurrentY);
	MessageText(TextLine, 10, 6, 0);
	sprintf(TextLine, "Process time: %02ld:%02ld:%02ld", ProcessHours, ProcessMinutes, ProcessSeconds);
	MessageText(TextLine, 10, 7, 0);

}
//------------------------- PrintMenu_Plot ------------------------------
