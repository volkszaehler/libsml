# libSML

[![Build Status](https://travis-ci.org/volkszaehler/libsml.svg?branch=master)](https://travis-ci.org/volkszaehler/libsml)
[![Join the chat at https://gitter.im/volkszaehler/volkszaehler.org](https://badges.gitter.im/volkszaehler/volkszaehler.org.svg)](https://gitter.im/volkszaehler/volkszaehler.org?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

libSML is a library which implements the Smart Message Language (SML) protocol specified by VDE's Forum Netztechnik/Netzbetrieb (FNN).
It can be utilized to communicate to FNN specified Smart Meters or Smart Meter components (EDL/MUC).

### This is the official libSML repo, the original dailab libsml repo on GitHub is dead 

### Usage
An example how to use libSML is in the examples directory.

#### Dependencies
Ubuntu

	apt-get install uuid-dev uuid-runtime

#### Compilation
 
	make

The resulting binaries are located in sml/lib

#### Testing
For testing data have a look at https://github.com/devZer0/libsml-testing

### License
Copyright 2011 Juri Glass, Mathias Runge, Nadim El Sayed - DAI-Labor, TU-Berlin
Copyright 2014-2018 libSML contributors

libSML is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

libSML is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

See the file LICENSE for the full license text.

### Thanks
Steffen Vogel, Thorben Thuermer, Daniel Pauli, He Bowei

#### Thirdparty Acknowledgements
This product includes software developed for the Unity Project, by Mike Karlesky, Mark VanderVoord, and Greg Williams and other contributors
