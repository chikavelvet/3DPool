# ThreeDPool with Networking
Group: **Ghost Babel**

- *Casey Bigelow*
- *Rishi Amarnani*
- *Trey Gonsoulin*

## Changes for Multiplayer
### Gameplay
To introduce multiplayer into our pool game, we can introduce a second stick (same cue ball), and count the number of balls each player has knocked in. The players take turns shooting the cue ball, and the player able to knock in the most balls wins. Traditionally, when a player knocks in a ball, they are allowed another shot without switching. This can also be potentially extended to alternate game modes, like assigning each player a color of ball to knock in, separating the balls into two colors, and determining the winning player by whoever can knock all balls of their color in first (with potentially a tertiary "8-ball" separate from either color to be knocked in last). The non-shooting player may be able to spectate the shot in a free-camera way.

### Networking
Because the players take turns shooting, there will not need to be an excessive amount of data sent via TCP during the shot, and there is an opportunity between shots to sync up positional data. One viable approach might be to send ball/stick positional data from the current shooting player to the other player via UDP during the shot, as this positional data can be slightly inaccurate without it harming the non-shooting player. Then, after the shot is finished, the final positional data can be synced with assurance via TCP before the next player begins their shot. This limits the cost of sending the large amount of changing data during shots, and limits the amount of data needed to be sent over the more costly TCP between shots. Score, pocket, and win data will need to be TCP synced. 

### Extra Features
Since we are extending our game from assignment 2, we will have more opportunity to add additional features (generally non-network features) we considered previously (like a trajectory guide, main/pause menu, etc.) For extra networking features, we could implement a high score and leaderboard system based on the amount of strokes used and have that be online (though this would require an external server tracking that information). 

## Labor Division
As we did on assignment 2, we will generally use pair programming principles and avoid segregating project portions too heavily (though some division of labor can improve efficiency). 

**Roles**:

- Casey:
	- Something
- Rishi:
	- Something else
- Trey:
	- A third thing

### Order of Attack
Our first goal is to get the networking libraries connected and working (including some basic functionality testing). Then, we plan to spend a considerable amount of effort refactoring our existing code to improve it in a number of ways: clean it up, separate unrelated things into their respective classes, and use component mentality, as well as reconsider the structure from a higher level. After that, we can get to work on the basic requirements, i.e., the networking. Finally, after all the main requirements are completed, we can work on extra features.
