OpenCV Face Detection
========================

GPU accelerated Face Detection using OpenCV. This tool can be run either GPU(CUDA) or ARM CPU without recompiling.
Command line arguments allows to specify which mode to run.  

Port the application to your environment:
>       $ aclocal

>       $ autoheader

>       $ autoconf

>       $ libtoolize --automake

>       $ automake --add-missing

>       $ ./configure

Build the application:
>       $ make

Note: This application require Boost - Program Option library. Following command to install it on Ubuntu.
>       $sudo apt-get install libboost-program-options-dev

Run the application:
>       $ cd bin

>       $ ./face_detection --mode gpu --classifier haarcascade_frontalface_alt.xml

>       $ ./face_detection --help
>           Face Detection implementation for embedded devices
>            Copyright 2015 NTU
>
>           Options:
>           -h [ --help ]               Print help messages
>           -v [ --version ]            Print program version
>           -m [ --mode ] arg           execution mode GPU or CPU
>           -c [ --classifier ] arg     path to classifier xml file
>           -b [ --show-biggest-face ]  Show biggest face in a separate window

You need a web cam to run this application.
