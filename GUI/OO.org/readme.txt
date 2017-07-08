OpenOffice.org FANN Plugin consist of two elements:
 *  Calc Add-in - adds a new menu to Calc which lets launch the client program.
    ('extension' directory)
 *  Client - main application part which is used to manage networks.
    ('client' directory)

The following build instruction is prepared for Linux, but building in Windows
with MinGW or on a Mac is possible and requires only minor changes in Makefiles and
environment setup procedures.

In order to build the elements we must follow 3 steps:

1.  ENVIRONMENT SETUP

    Download required packages:
     *  OpenOffice.org 2.2
     *  OpenOffice.org 2.2 SDK
     *  Qt 4.X development libs
     *  FANN library

    Set some environment variables:
      export OFFICE_HOME=<OpenOffice.org 2.2 dir>
      export OO_SDK_HOME=<OpenOffice.org 2.2 SDK dir>
      export LD_LIBRARY_PATH="$OFFICE_HOME/program:$LD_LIBRARY_PATH"

    Run OO.org SDK configure script:
      $OO_SDK_HOME/setsdkenv_linux 

    We also need FANN lib:
      export FANN_SRC_DIR=<FANN source dir>
      export FANN_LIB_DIR=<FANN library path>

    It's assumed that the program links against static FANN lib file, named libfann.a.
    To alter build settings please change the contents of plugin.pro file to suit
    your system configuration.

    IMPORTANT!
    The aforementioned values are mostly necessary only for building the plugin.
    However OFFICE_HOME variable must be set properly to execute the program at all.

2.  OPENOFFICE.ORG SETUP

    Open the file $OFFICE_HOME/share/registry/data/org/openoffice/Setup.xcu and
    check whether <node oor:name="Office"/> contains the following section:
      <prop oor:name="ooSetupConnectionURL"> 
         <value>socket,host=localhost,port=8100;urp;StarOffice.ServiceManager</value>
      </prop>
    This is necessary as the default OpenOffice.org behaviour is not to listen
    on any socket nor pipe.

3.  BUILDING THE APPLICATION

    In the client directory run:
      qmake

    In the program top directory run:
      make
      sudo make install        

    Sometimes, especially when soffice process is not running, after registering the 
    extension unopkg throws a bad_alloc exception though registration gets successfully 
    completed. (The same sometimes occurs with the reference examples in SDK)
    In that case issue following commands in the program top directory:
      make oo_extension
      make oo_client
      sudo make install
    
    NOTE:
    The client build process produces tons of warnings about lack of virtual destructors in 
    some polymorphic classes. This is a bug in OpenOffice.org code and can be resolved
    only by turning -Wall off when compiling. 

    
To run the plugin open Calc and Search for FANN menu. Then click on the 'Console' item ;)

