# pd-vscode-wsl
## Installation
### WSL
If you don't have WSL enabled already, head over to https://docs.microsoft.com/de-de/windows/wsl/install-win10 and follow the instructions. We will use the Ubuntu 20.04 LTS distribution from <https://www.microsoft.com/en-us/p/ubuntu-2004-lts/9n6svws3rx71> (though other Ubuntu distributions should work too)

### VSCode
Download VSCode from https://code.visualstudio.com/download and install the following extensions:
- [Remote - WSL](vscode:extension/ms-vscode-remote.remote-wsl)
- [C/C++ Extension Pack](vscode:extension/ms-vscode.cpptools-extension-pack)
- [Task Shell Input](vscode:extension/augustocdias.tasks-shell-input) (needed for debugging)


### Pure Data
Download the Installer Version of Pure Data from <https://puredata.info/downloads/pure-data>

## Setup
Open up VSCode and connect to your WSL distribution. Press `CTRL+ö` to bring up a linux bash Terminal.

Here we will enter the following commands to install all required new packages:
```
sudo apt update -y && sudo apt upgrade -y

sudo apt install -y build-essential mingw-w64 gdb make puredata
```

And clone the project repository:
```
git clone https://github.com/schlegelflegel/pd-vscode-wsl

cd pd-vscode-wsl

code .
```

VSCode will open the repository in a new window and might ask you to install the recommended extensions, if you haven't done so already.
