# Yuuki Language  ![](https://api.travis-ci.com/AlaricGilbert/yuuki-lang.svg?branch=master) ![](https://ci.appveyor.com/api/projects/status/github/AlaricGilbert/yuuki-lang?svg=true)

* A super light weight language that supports generic OOP programming which developed during study compile principle.

## TODO-List
* [x] Lexer
* [ ] Parser
* [ ] Semantic Analyse
* [ ] Code Generation
* [ ] VM, runtime, etc...
* Please be aware that the project is still in early development phase.
## Build 
* Please check your compiler compatibility with `c++ 17` before compile
  * We need at least `c++ 17` feature `std::string_view` in all platforms
  * We need `c++ latest` in Windows/VS toolchains

### macOS build instructions
* Install command-line tools:
```shell script
$ xcode-select --install
```
* Install CMake:
```shell script
$ brew install cmake
```
* Completely clone the repo in a recursively mode:
```shell script
$ git clone --recursive https://github.com/AlaricGilbert/yuuki-lang
``` 
* Go into the directory, run the compile script:
```shell script
$ cd yuuki-lang
$ ./build.sh
```

### Linux build instructions
* Install tools (Example code in Ubuntu, GCC toolchain)
```shell script
$ apt-get update
$ apt-get insatll gcc g++ cmake
``` 
* Completely clone the repo in a recursively mode:
```shell script
$ git clone --recursive https://github.com/AlaricGilbert/yuuki-lang
``` 
* Go into the directory, run the compile script:
```shell script
$ cd yuuki-lang
$ ./build.sh
```
* Then you can find results of compilations under `artifacts` folder.

### Windows build instructions
* Goto CMake website, install CMake and add CMake into `PATH`

* Completely clone the repo in a recursively mode:
```powershell
PS git clone --recursive https://github.com/AlaricGilbert/yuuki-lang
``` 
#### Windows with MSVC tool chain
**We assume that MSVC was installed correctly in this context**

* Go into the directory, run the compile script (**USE POWERSHELL**):

```powershell
PS cd yuuki-lang
PS build.ps1
```
*You may need to modify bypass execution settings for your Powershell*

#### Windows with MinGW in PATH
**We assume that MinGW (win32) was installed correctly in this context and added into `PATH`**
* Go into the directory, run the compile script:
```powershell
PS cd yuuki-lang
PS mkdir build
PS cd build
PS cmake .. -G"MinGW Makefiles"
PS make
```

## LICENSE
This  project is licensed under MIT with below exceptions:
* You are not allowed to directly submit code this repo as *your* coursework.

## Contribution
Pull requests are welcomed!