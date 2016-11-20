# NAMELESS 2D ENGINE

NAMELESS 2D is a 2D (very WIP) RPG-like game engine for the Nintendo 3DS. I will update it regularly (or not) to add new function such as NPCs, levels, battles
or whatever. The final aim is to create a full original homebrew game.  
__*THIS ENGINE IS NOT COMPLETE!!!!*__

## 3DS you say?

Yep, because why not? :P I like this system and the use of the two screens could be useful in certain parts of a game! The project is
opened to everyone with basic C++ and lua programming.

## How will it work?

The main engine will be written in full C++. However, the engine will base itself on LUA scripts and external images, musics and
everything. So the games will be entirely moddable!

## What are the features as for now?

   * A player moveable with D-PAD and Circle-Pad (made with paint)
   * Working buttons
   * Colorful text!
   * A menu with multiple options!
   * Sound: BGM and SFX
   * A START menu with a fancy animation

## What are the planned features?

  * Textboxes
  * Battles
  * Inventory
  * NPCs
  * Maps
  * Scripts

## Building and compiling

 You will need [DEVKITPRO](https://devkitpro.org/) installed. You'll then need to install [sf2dlib](https://github.com/xerpi/sf2dlib)
 , [sfillib](https://github.com/xerpi/sfillib) and [sftdlib](https://github.com/xerpi/sftdlib). Make sure you have everything insatlled, and then run the command  
 `make`
 If your 3DS is connected to the internet, you can also edit Makefile to make it match with your 3DS's IP, then open FBI, go to
 "Network installation", and then on your PC run the command
 `make spunch`
 The CIA will magically be installed on your 3DS! Make sure you have Java installed to run this command. You will also need a
 "NAMELESS" folder at the root of your SD card that contains the data needed.


## CREDITS


 * Smealum for ctrulib, *hax and homebrew possibilities
 * Xerpi for his port of portlibs, sf2dlib, stfd and sfill
 * TricksterGuy for his [3DS homebrew template](https://github.com/TricksterGuy/3ds-template)
 
Don't forget to credit me if you use my work, and if you don't, __*I HOPE YOUR BODY IS READY*__   :godmode:
