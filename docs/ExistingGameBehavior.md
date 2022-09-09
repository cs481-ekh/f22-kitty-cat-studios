# Existing Game Behavior
This document's purpose is to document the current game behavior (09/09/2022) in order to analyze and understand the steps that need to be taken next to add polish to the game.

## Car Behavior
The car can drive forward and backwards using the 'W' and 'S' keys, it looks like there's no way to turn the car unless you are moving forward or backwards. Ocassionally there is a sound effect when the car is speeding up in either direction. When jumping there is a when going up but no sound effect when landing. When the car is in the air the rotation/tilt of it can be controlled. If you die while moving, the moving sound effect continues to play until you respawn. When hitting an enemy or getting hit, there is no indication that you have been hit or that the enemy has been hit. The health bar at the bottom is the only indicator. When shooting, the gun will lock on to the nearest enemy and when firing it will shoot that car. There doesn't seem to be very much feedback when it comes to shooting. There is a sound effect but that's about it. If no enemies are near, the car will shoot straight forward. There were certain points that I got stuck due to the cars controls.

## Game Loop
At the beginning of every level, there is an opening cutscene sequence that shows off the entire map. This is a nice touch but gets pretty annoying after the 3rd time you see it. The way the game progresses from the main menu is by selecting play, from there, every time you win a map, you progress to the next map. Every level I palyed seemed to have the same number of enemies that needed to be defeated "3". There was very little challenge in playing through these levels and I beat them fairly quickly. When dying, a respawn UI timer pops up. At the end of the timer, the player respawns. When I was playing through the game, at one point the timer got stuck on 1 second and I had to quit out of the game. Each level has a timer that ticks down. When the timer reaches '0', a "You Failed" screen pops up. You can either return to the Main Menu or replay the level.

## Powerup Behavior

## Map Elements

## Game UI
