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

#ifndef CDROM_PLOTTER_PLOTTER_H
#define CDROM_PLOTTER_PLOTTER_H

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

/* ---- Menu Level Constants -----------------------------------------------------------------------------------------------*/

#define MENU_MAIN 0
#define MENU_FILE 1
#define MENU_PLOTTER 2

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


#endif //CDROM_PLOTTER_PLOTTER_H
