# Project: CD ROM plotter
# Homepage: www.HomoFaciens.de
# Author Norbert Heinz
# Modified by Ian McLinden
# Version: 0.5
# Creation date: 28.09.2015
# Modification date: 22.12.2016
# This program is free software you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation version 3 of the License.
# This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
# For a copy of the GNU General Public License see http://www.gnu.org/licenses/
# 
# For details see:
# http://www.HomoFaciens.de/technics-machines-plotter-cdrom_en_navion.htm

CC=gcc
CFLAGS= -O2 -g -Wall -I/usr/local/include -L/usr/local/lib -lwiringPi -lm

###############

# -------- Compilation ----------------------------------------------------------------------------------------------- #

all:	plotter

plotter:	plotter.o
	$(CC) $(CFLAGS) plotter.o -o plotter

plotter.o:	plotter.c plotter.h
	$(CC) $(CFLAGS) -c plotter.c -o plotter.o

clean:
	rm -Rf *.o *~ *.bak *.out *.dSYM plotter


# -------- Execution ------------------------------------------------------------------------------------------------- #

run:	all
	./plotter
