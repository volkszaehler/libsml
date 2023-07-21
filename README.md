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

#### Debian Packages

[![Hosted By: Cloudsmith](https://img.shields.io/badge/OSS%20hosting%20by-cloudsmith-blue?logo=cloudsmith&style=flat-square)](https://cloudsmith.com)

We now build Debian packages for amd64, armhf and arm64 as part of our releases.
The ones attached to the release are meant for trixie. These and packages for 
bookworm and bullseye are also provided through a repository graciously provided by 
[Cloudsmith](https://cloudsmith.com).

The setup of the repository is also 
[explained by Cloudsmith](https://cloudsmith.io/~volkszaehler/repos/volkszaehler-org-project/setup/#formats-deb).
This boils down to adding a file to /etc/apt/sources.list.d/ containing
```
deb [signed-by=/usr/share/keyrings/volkszaehler-volkszaehler-org-project-archive-keyring.gpg] https://dl.cloudsmith.io/public/volkszaehler/volkszaehler-org-project/deb/debian bookworm main
deb-src [signed-by=/usr/share/keyrings/volkszaehler-volkszaehler-org-project-archive-keyring.gpg] https://dl.cloudsmith.io/public/volkszaehler/volkszaehler-org-project/deb/debian bookworm main
```
(replace bookworm with your current distro) and retrieving the key as a trusted one
```
curl -1sLf "https://dl.cloudsmith.io/public/volkszaehler/volkszaehler-org-project/gpg.21DBDAC56DF44DA1.key" | \
	gpg --dearmor > /usr/share/keyrings/volkszaehler-volkszaehler-org-project-archive-keyring.gpg
```

There is also a libsml package that is part of Debian. While the version in
older Debian releases is somewhat outdated the version in trixie is recent. 
Other than the project packages the official Debian package has a version 
postfix separated by a hyphen (e.g. 1.0.2-3). It has minor differences in 
packaging and is maintained as part of the project in the debian branch.

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
