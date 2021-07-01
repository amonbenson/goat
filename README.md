# pd-vscode-wsl
This repository is intended as a starting point for compiling Pure Data externals for Windows or Linux using WSL (Windows Subsystem for Linux), VSCode and the MinGW W64 cross compiler.

If you don't have any experience with WSL or VSCode, I'd **not** recommend you this method, because it involves quite a few more steps than just using a (real or virtual) Linux Machine. On the other hand, you will be able to compile both linux and windows externals while using your existing WSL setup.

## Installation
### WSL
If you don't have WSL already enabled, follow the instructions [here](https://docs.microsoft.com/de-de/windows/wsl/install-win10). Make sure to use WSL 2. We will use [Ubuntu 20.04 LTS](https://www.microsoft.com/en-us/p/ubuntu-2004-lts/9n6svws3rx71), though other Ubuntu distributions should work too. If you've just installed the app, you'll need to launch it once to finish the installation and set the username / password.

### Pure Data
[Download](https://puredata.info/downloads/pure-data) the Windows Installer Version of Pure Data.

### VSCode
[Download](https://code.visualstudio.com/download) VSCode and install the following extensions:
- [Remote - WSL](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-wsl)
- [C/C++ Extension Pack](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools-extension-pack)
- [Task Shell Input](https://marketplace.visualstudio.com/items?itemName=augustocdias.tasks-shell-input) (needed for debugging)

Open up VSCode and connect to your WSL distribution (<kbd>CTRL</kbd>+<kbd>Shift</kbd>+<kbd>P</kbd> > `Remote-WSL: New WSL Window`). Then press <kbd>CTRL</kbd>+<kbd>ö</kbd> to bring up a linux bash Terminal. Here we will enter the following commands to install all required new packages:
```
sudo apt update -y && sudo apt upgrade -y

sudo apt install -y build-essential mingw-w64 gdb make puredata
```

Next we can setup the project folder:
```
mkdir your-project-name

cd your-project-name

git clone -j8 --recurse-submodules https://github.com/schlegelflegel/pd-vscode-wsl .

code .
```

VSCode will open up in a new window and might ask you to install the recommended extensions, if you haven't done so already. You will find the code to a basic hello world example in `src`.

## Building
There are two makefiles in the project:
- `Makefile` is the main one, where all sources are listed, the library name is set and pd-lib-builder is included.
- `Makefile.wsl` overwrites the compiler and Pd path definitions, so that the MinGW toolchain is used. Depending on your Pd install location and if you are on a 32 bit system, you might need to change those values. (See [pd-lib-builder paths](https://github.com/pure-data/pd-lib-builder#paths) and `Makefile.wsl` for more information)

To build a linux external, in the integrated Terminal (<kbd>CTRL</kbd>+<kbd>ö</kbd>) type:
```
make clean all
```

To build a windows external, use
```
make -f Makefile.wsl clean all
```

You can also directly install the external:
```
make -f Makefile.wsl clean install
```
To change the installation path, update the `PDLIBDIR` variable in `Makefile.wsl`

I noticed that some of the signal examples have the wrong naming convention (`rtap_osc.c` instead of `rtap_osc~.c`) which will prevent Pure Data from loading them. When compiling signal externals the c file must end with a tilde symbol `~` and all class methods in that file must include the suffix `_tilde`.

## Debugging
Debugging is a bit more complex with this setup and I didn't manage to get the debugger working with the Windows Pure Data Installation (If anyone does, let me know :D).

What we can do however, is to start the Ubuntu version of Pure Data directly withing WSL. For this to work, we need to install an X-Server on Windows to have the GUI showing up and we won't have any audio output (when debugging with breakpoints and stuff, realtime audio output wouldn't make much sense anyway).

### Installing VcXsrv
[Download](https://uni-tuebingen.de/fakultaeten/wirtschafts-und-sozialwissenschaftliche-fakultaet/faecher/fachbereich-wirtschaftswissenschaft/wirtschaftswissenschaft/fb-wiwi/einrichtungen-wirtschaftswissenschaft/wiwi-it/services/services/computing-asp/tools/x-server/vcxsrv/) and install the Windows X-Server "VcXsrv".

Start the "XLaunch" Application. Select **"Multiple windows"** and **Display number 0**. Click next twice and leave all the default settings. At this point you should restart your PC. Then wait for the X-Server to start automatically in the background (see the X-symbol in the notification area) and only then open up VSCode again.

### Starting the debugger
Back in your VSCode project, press <kbd>F5</kbd> to launch the debugger. If everything goes well, this should automatically build an ubuntu external with debug flags (`make clean alldebug`), get the ip address of your host machine to set the `DISPLAY` environment variable and finally launch Pure Data. If Pure Data cannot connect to the display, try restarting your PC and wait for XLaunch to start *before* opening VSCode.

You can now set breakpoints in your project. Keep in mind, that breakpoints will stay greyed out until the external's shared library is loaded, e. g. until you add your first "goat" object box in Pd.
