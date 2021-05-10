# pd-vscode-wsl
This repository is intended as a starting point for compiling Pure Data externals for Windows or Linux using WSL (Windows Subsystem for Linux), VSCode and the MinGW W64 cross compiler.

If you don't have any experience with WSL or VSCode, I'd **not** recommend you this method, because it involves quite a few more steps than just using a (real or virtual) Linux Machine. On the other hand, you will be able to compile both linux and windows externals while using your existing WSL setup.

## Installation
### WSL
If you don't have WSL already enabled, follow the instructions [here](https://docs.microsoft.com/de-de/windows/wsl/install-win10). We will use [Ubuntu 20.04 LTS](https://www.microsoft.com/en-us/p/ubuntu-2004-lts/9n6svws3rx71), though other Ubuntu distributions should work too.

### Pure Data
[Download](https://puredata.info/downloads/pure-data) the Windows Installer Version of Pure Data.

### VSCode
[Download](https://code.visualstudio.com/download) VSCode and install the following extensions:
- [Remote - WSL](vscode:extension/ms-vscode-remote.remote-wsl)
- [C/C++ Extension Pack](vscode:extension/ms-vscode.cpptools-extension-pack)
- [Task Shell Input](vscode:extension/augustocdias.tasks-shell-input) (needed for debugging)

Open up VSCode and connect to your WSL distribution (`CTRL+Shift+P` > `Remote-WSL: New WSL Window`). Then press `CTRL+ö` to bring up a linux bash Terminal. Here we will enter the following commands to install all required new packages:
```
sudo apt update -y && sudo apt upgrade -y

sudo apt install -y build-essential mingw-w64 gdb make puredata
```

Now clone this project repository:
```
git clone https://github.com/schlegelflegel/pd-vscode-wsl

cd pd-vscode-wsl

code .
```

VSCode will open the repository in a new window and might ask you to install the recommended extensions, if you haven't done so already. You will find the source code to a basic hello world example in `src`.

## Building


## Debugging
