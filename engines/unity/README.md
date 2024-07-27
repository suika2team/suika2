How to build
============

## How to build for Gaming Consoles
* Open the terminal that is capable for the C compiler of the console's SDK.
* For PlayStation 4/5:
  * Edit `ps45.mk` and change `CC=` and `AR=`
* For Xbox Series X|S:
  * Edit `xbox.mk` and change `CC=` and `AR=`
* For Switch:
  * Edit `switch.mk` and change `CC=` and `AR=`
* Run `make`
* Open the project by Unity

## On Unity
* On `Player Settings`, check `Allow unsafe code`.
* Double click `MainScene`
* Tie `Script` to `OpenNovelScript.cs`
* Tie `BGM`, `SE`, `Voice` and `SYSSE` to `OpenNovelAudio.cs`
* Run
