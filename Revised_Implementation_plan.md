# CS354R Assignment 4 Milestone 4.1
Group: **No Good Devs**

- *Casey Bigelow*
- *Rishi Amarnani*
- *Trey Gonsoulin*

# Features

## Core Feature: AI

#### General Idea
We will build articial intelligence to play pool against the player in a turn-based game
in the same environment. The player will be able to see the AI move the cue stick and hit
the ball. On a tangential note, this turn-based change will also have to be applied to 
multiplayer.

#### Greedy Algorithm

Our general idea is to make a greedy algorithm that decides
which ball to hit into which pocket based on two heuristics:
- the proximity of a ball to a pocket
- how "straight" a shot is

We found that these are the two most important factors that we think about when we
play the game, and so we will use these two factors to build our AI to make it realistic.

#### Calculating Cue-Ball Hit Direction
After using our greedy algorithm to decide which ball to hit and which pocket to hit it
into, we'll know *where on the chosen ball the cue ball must touch*. 

From this position, we can easily calculate the exact direction that we have to hit
the cue ball at. Then the AI just charges up the cue stick and hits the ball in the
calculated direction.

#### Raycasts
We will perform two types of cylindrical raycasts to find clear shots.
- Raycast between cue-ball and ball we want to hit
- Raycast between ball we want to hit and pocket we want to hit into

We will run the greedy algorithm on a subset of the balls and pockets
that pass these raycasts.

## Extra Feature: Difficulty Settings

#### General Idea
Because we are implementing AI, we will also implement difficulty settings for the AI.
These settings will be easy, medium, hard, and adaptive. 

#### Implementation: Error Factor
To implement these settings, we will have an error factor which will be increased as you
go from hard to medium, and medium to easy. This error factor will be multilpied by a 
random number, and the result will be added to the direction the AI wants to hit the 
cue ball.

The ball the AI chooses to hit will not change as difficulty settings change.
All that will change is the precision of the shot. This makes the AI realistic, 
because generally speaking people of all skill levels will choose the same shot, 
but some people will be more skilled and will be able to execute the shot more precisely. 

#### Adaptive Difficulty
The adaptive setting will take into account the performance of the player as the game goes
on and adjust the AI to easy, medium, or hard dynamically.

Since we care about the performance of the player as the game goes on, we will consider the
performance of the player to be equivalent to the number of balls hit in the last few shots. 

## Core Feature: Turn-based Multiplayer
Currently our multiplayer runs two games separately and sends scores over the network. So
when you start a network game, you are just racing against your opponent to see who can
get all of the balls in first. Now, we want to change it so that you are playing a turn-based
game against your opponent in the same pool table environment.

## Extra Feature: Passive Networking
We want to implement passive networking for the player to be able to detect when others are playing
on the same LAN. So, when you are playing a single-player game against an AI, a message might pop up
saying that someone else is playing a single player game on the same LAN. 

While playing single player, if someone else is playing on the same LAN, you will be able to invite them
to a multiplayer game. They will get the invite from you. Until they accept the invite, you can keep
playing single-player. If they reject the invite, you will be notified.

# Team and Work Division
We are keeping the same team as before (Rishi, Casey, and Trey). All of the features we are attempting to
add are large-scale and will require lots of designing and programming. So we will try as much as possible
to use pair-programming and to not divide up roles. For now, all of us will work on all aspects of the project.

# Attack Plan
We will start with implementing the core features which include the turn-based model for both single player and multiplayer, and
the AI. After implemeting these features, we can add on difficulty settings, and the passive multiplayer, which are extra features. 
