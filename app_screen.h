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

#ifndef CDROM_PLOTTER_APP_SCREEN_H
#define CDROM_PLOTTER_APP_SCREEN_H

int gotoxy(int x, int y);
void clrscr(int StartRow, int EndRow);

int kbhit(void);
int getch();

void MessageText(char *message, int x, int y, int alignment);
void PrintRow(char character, int y);
void ErrorText(char *message);

void PrintMenu_Main(char *PlotFile, double scale, double width, double height, long MoveLength, int plotterMode);
char *PrintMenu_File(int StartRow, int selected);

void PrintMenu_Plot(char *FullFileName, long NumberOfLines, long CurrentLine, long CurrentX, long CurrentY,
                    long StartTime);

#endif //CDROM_PLOTTER_APP_SCREEN_H
