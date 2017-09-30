# CS354R Assignment 2 Design Document

## Game Design
### Overview
3D pool. Balls are suspended in a three dimensional environment (rectangular prism). Use a cue (/paddle) to hit the balls into one of 12 pockets in the corners/edges of the room. You must hit a cue ball with the cue/paddle which then hits another ball into a pocket.

### Scoring Rules
Golf-style stroke system. Try to hit all the balls in the pockets with as few strokes as possible. Hitting the cue ball into a pocket is a +1 penalty. Hit the 8-ball last or you lose.

### GUI
Display stroke count. Display high score. Display which balls are left. Display power meter. Display win/lose splash and restart button. Display high score screen.

### Controls
Move mouse around to rotate cue around cue ball (3-dimensionally, fixed distance away from cue ball). Left-click down will lock in position, moving mouse backward or forward will increase/decrease power, left-click release will hit the ball with the cue. At beginning of game and if the cue ball is hit in a hole, use WASD to move cue ball initial position.

### Camera
Camera locked on cue ball when setting up hit. After hitting, camera moves out to a global view to watch the balls move. When all balls stop moving, camera goes back to cue ball.

### Sound
Pool sounds. We can record our own pool sounds in Jester/Union. There are probably free sounds online. Sound dependent on the force at which the balls are hit.

### Extra Features
(stretch goals)
 * Different difficulty settings
	* Adding small gravity to pockets to make getting ball in easier
 * Prediction line for ball path
 * Non-spherical/standard balls/cue
 * GUI menu for configuration
 * Main menu/title screen
 * Game saving/loading
 * Pausing/pause menu

## Software Architecture

### Data Heirarchy
Ball class with enumeration for the different kinds of balls (cue, 8, regular). Pocket class with information about pockets.

### Modularity
XML/Json file with information about the starting positions of balls, the number of balls, pocket position/size/orientation, etc., that can be modified. 

### Loops
#### Game Loop
Mediates between Ogre and Bullet. The game loop will keep track of important state, will send/receive information from the simulation loop (Bullet), and will send out information to the rendering loop (Ogre). Events from the simulation loop will trigger flags that will be dealt with in the game loop.

#### Rendering Loop
Renders frames based on information from the game loop.

#### Simulation Loop
Deals with collision detection, ball movement, and other physics things. Communicates with the game loop via events.

### Game State
Game state will be kept track of by the game loop in various appropriate data structures (vector for balls, vector for pockets).

### Work Estimation
The hardest part will be figuring out Bullet and integrating the game and simulation loop. The game state and game logic shouldn't be too hard, as well as camera movement and controls.

### Scale-back plan
Cutting out extraneous GUI. Scale back the extra features.

## Labor Division
### By Person
Casey:
 * Textures/graphics

Rishi:
 * Bullet
 * Finding sounds

Trey:
 * Game loop
 * Data structures

All:
 * GUI
 * Bullet Integration
 * Extra features

### Tools for Parallel Work
 * Git
 * Slack
 * Trello
 * Pair Programming

### Meeting Up Plan
We have decided that Tuesday and Thursdays are generally better, though MWF can work after 5. Weekends may be fine too. On each meetup, we will schedule the next meet up. 



