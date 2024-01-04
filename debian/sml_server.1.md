% SML_SERVER(1) Volkszaehler project | User Commands
% Joachim Zobel <jz-2017@heute-mogen.de>

# NAME

sml_server - makes libsml functionality available on the command line

# SYNOPSIS

**sml_server** [**-v**] [**-h**] [**-s**] _device_|_file_

# DESCRIPTION

The sml_server reads binary SML data from the given device or file. If a - is 
given, stdin is used. This data is then displayed in human readable form.

**-v**
:     This enables verbose output.

**-h**
:     This shows the help.

**-s**
:     will print the first sml message and then exit (single)

# EXAMPLES

Assuming there is a meter connected to ttyUSB0 you can display the transmitted 
data.

~~~
$ sml_server /dev/ttyUSB0 
129-129:199.130.3*255#ISK#
1-0:0.0.9*255#06 49 53 4b 01 0b cb 28 e6 1d #
1-0:1.8.0*255#10973297.0#Wh
1-0:1.8.1*255#10973297.0#Wh
1-0:1.8.2*255#0.0#Wh
1-0:16.7.0*255#144#W
...
~~~

# NOTE

The sml_server is intended as a tool for development and testing and as an 
example. It was not developed for production use.

# COPYRIGHT

The sml_server program is Copyright (C) Juri Glass, Mathias Runge, Nadim El 
Sayed and others.

# SEE ALSO

[libsml-testing](https://github.com/devZer0/libsml-testing)
is a collection of test data from real life smart meters. It also has
usage examples.

