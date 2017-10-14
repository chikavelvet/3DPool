# CS354R Assignment 2 Revised Implementation Document

## Controls/Instructions
Compile with standard `buildit`. Game binary is called `ThreeDPool`. 

**Controls**:
* Mouse - orbit cue stick around ball to choose direction
* Left-Mouse Down - Start charging
* Mouse forward and backward - change charge amount
* Left-Mouse Up - Release and hit cue ball
* T - toggle Free Look mode (WASD and mouse to move and look around)

## Components

### The Pool Cue
It's pretty close. Currently, you can rotate it horizontally and vertically around the cue ball (orbiting). You can hold left click and move the mouse back and forth to charge up the cue stick (it moves back and forth) and release the mouse button to hit the cue ball. There are min/max limits on the charge.

**To-do**:
* The camera doesn't move with the pool cue (i.e., first person-esque)
* Still needs to not interact with objects other than the cue ball
* Make it look nicer
* Random bug where cue can't hit ball

### The Balls
Works pretty well. The cue ball moves when hit, and collides correctly with walls and other balls. 

**To-do**:
* Texturing
* Add more balls
* Implement pyramid setup

### The "Table"/Walls
Textures work, and all the walls work with physics. 

**To-do**:
* Change texturing

### Pockets
Not implemented yet. There is space for them in the current table setup.

**To-do**:
* Make them
* Make collision behave appropriately

### Sound
Not implemented yet. 

**To-do**:
* Put them in the game.
* Try to make the initial break not too awful
	* Lower force -> less sound

### GUI
Not implemented yet. Not in the demo, but we are starting on it.

**To-do**:
* Add the GUI elements
* Prioritize scoring

## Problems We've Faced
**Problems**:
* Bullet integration
* Not using the given starter code initially

## Labor Division
*Casey*:
 * Textures/graphics
 * GUI

*Rishi*:
 * Bullet
 * Finding sounds

*Trey*:
 * GUI

*All*:
 * Bullet Integration
 * Extra features

