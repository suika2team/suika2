How To Build
============

## Prerequisite

* Windows 11 and Unity Editor 2022.3 are required

## Instructions

* Open a project by Unity Editor
* In `Player Settings` dialog, check `Allow unsafe code`
* Double click `MainScene`
* Tie `Script` to the script `OpenNovelScript.cs`
* Tie `BGM`, `SE`, `Voice`, and `SYSSE` to the script `OpenNovelAudio.cs`
* Run preview

## About Custom DLL

* You need a custom DLL to run on a game console
* Please recompile `libopennovel.dll` by a target SDK
* There are some examples in `suika2/engines/unity/Makefile`
