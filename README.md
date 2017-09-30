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

## Software Architecture

### Data Heirarchy
Ball class with enumeration for the different kinds of balls (cue, 8, regular). Pocket class with information about pockets.


## Labor Division
