# CS354R Assignment 3 Milestone 3.2
Group: **Ghost Babel**

- *Casey Bigelow*
- *Rishi Amarnani*
- *Trey Gonsoulin*

## Controls/Instructions
Compile with standard `buildit`. Game binary is called `ThreeDPool`. 

**Controls**:
* Mouse - orbit cue stick around ball to choose direction
* Left-Mouse Down - Start charging
* Mouse forward and backward - change charge amount
* Left-Mouse Up - Release and hit cue ball
* T - toggle Free Look mode (WASD and mouse to move and look around)

For multiplayer:
On one computer, in the multiplayer menu, hit "Host Game". Then, on the second computer, hit "Join Game" and type in the IP Address shown on the host computer. Then hit "Go" to start the game!

## Progress

### Networking
Networking is working! We are currently able to send scoring information back and forth between both players and synchronise it. Currently, the engines on each computer process physics separately, which could be a security/cheating risk, though for this project it doesn't matter as much. We may work on having physics data sent between the client and host as well.

### GUI
We have a main menu and connection lobby. 

## Plan Moving Forward
Essentially, we have the components of the finished project done. This is good, as we have the next week to clean up our code, refactor and optimize, and add extra features! We've already put some work into prettifying the GUI and textures, and it already looks much better than our project 2. We have plenty of things in mind for that. Our code also could use a good steam cleaning and restructuring, to be honest.

**Roles**:

- Casey:
	- Textures/graphics
	- Sound
	- Extra Features

- Rishi:
	- Networking
	- Extra Features

- Trey:
	- GUI
	- Refactoring
	- Extra Features


