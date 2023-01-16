# Pillow Talk 

## What is Pillow Talk? 

Pillow Talk is a program written in Python that allows to use a game controller or joystick as a means to control your Linux PC. 

## How it Works

This program heavily depends on [qjoypad](https://github.com/panzi/qjoypad "qjoypad"). When you press a button on your game controller, qjoypad will send keyboard output to your desktop envoirnment; Pillow Talk then takes that keyboard input and 'translates' it to a Python function. 

### Example

Game Controller     > qjoypad		> Pillow Talk                     > System Output 

X button is pressed > sends "1" key > Pillow Talk listens for "1" key > hello-world() is executed.

## Installation

As it stands, Pillow Talk is my own custom configuration and isn't designed for others to be able to easily adapt it to their systems. 

## Dependencies

* qjoypad (for game controller input)
* florence (for onscreen keyboard)

## Further Development 

In future iterations of Pillow Talk, the configuration would be something more like running a script on the qjoypad config file which would auto-generate a Pillow Talk config file, which then could be edited to run python/bash scripts. 