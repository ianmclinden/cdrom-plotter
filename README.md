# CD ROM Plotter
### Based on [original project & source code](http://www.homofaciens.com/technics-machines-plotter-cdrom_en_navion.htm) by [Norbert Heinz](http://www.homofaciens.com/imprint_en_navion.htm)

* Modified by Ian McLinden
* Version: 0.5
* Creation date: 28.09.2015
* Modification date: 22.12.2016

## License:
This program is free software you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation version 3 of the License.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

For a copy of the GNU General Public License see http://www.gnu.org/licenses/

## Dependencies:

### WiringPi

install wiringPi (if not already installed):

```
sudo apt-get update
sudo apt-get install git-core
git clone git://git.drogon.net/wiringPi
cd wiringPi
git pull origin
./build
```

## Compilation
The program includes a Makefile compatible with the GNU make utility.
To compile the program, simply use the command `make all` from the command
line, inside the project top level directory.

If you wish to manually compile the program, you must also link all the
appropriate files. To do this, run the following command:

`gcc plotter-cdrom.c -I/usr/local/include -L/usr/local/lib -lwiringPi -lm -o plotter-cdrom`
	
This will have the same effect as running the `make all` command,
discussed above.

## Shell Use

To use the program from the command line, the program must be compiled.
See the section titled "Compilation" for more information.

Once the program is compiled, and the executable file 'plotter' exists
in the project root directory, simply use the command,
like any other executable: by running

`./plotter`

## Authors

* Original Codebase by [Norbert Heinz](http://www.homofaciens.com/imprint_en_navion.htm)
* Modified by [Ian McLinden](https://github.com/ianmclinden)