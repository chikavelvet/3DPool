# CS354R Assignment 4 Milestone 4.1
Group: **No Good Devs**

- *Casey Bigelow*
- *Rishi Amarnani*
- *Trey Gonsoulin*

- AI
- Difficulty Settings 
- Upgraded multiplayer (play against each other in the same environment)
- Passive networking during single player
- Additional Refactoring

## AI
We will build articial intelligence to play pool against the player.
Our general idea so far is to made a greedy algorithm based on two heuristics:
	- the proximity of a ball to a pocket
	- how "straight" a shot is	

We found that these are the two most important factors that we think about when we
play the game, and so we will use these two factors to build our AI to make it realistic.

We will also perform cylndrical raycasts between the cue-ball, the balls, and the pockets
to find clear paths to balls.

In order for AI to make sense for our game, we will have to change our fundamental game
design to be turn based. This change will also have to be applied to multiplayer.

## Difficulty Settings
Because we are implementing AI, we will also implement difficulty settings for the AI.
These settings will be easy, medium, hard, and adaptive. 

Easy, medium, and hard settings will change 

## Plan Moving Forward
We are going to build on top of our existing game which we have 

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


