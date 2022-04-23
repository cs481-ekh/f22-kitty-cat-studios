# BroncoDrome

[![BroncoDrome Video](https://img.youtube.com/vi/WcRSF955N7A/0.jpg)](https://www.youtube.com/watch?v=WcRSF955N7A)

## Current Contributors

| Name             | Role                                   |
| ---------------  | -------------------------------------- |
| Ben Bettencourt  | Game Loop and Mechanics Programmer     |
| Saulo Gragg      | Modeler and Mechanics Programmer       |
| Joey Kitzhaber   | Game Tester and Mechanics Programmer   |
| Doug McEwen      | Game Tester and Audio Programmer       |
| Marie Phelan     | AI and Mechanics Programmer            |
| Ross Rippee      | Game Loop and Mechanics Programmer     |
| DJ Roychourdhury | AI and Mechanics Programmer            |

## Previous Contributors

| Name            | Role                              |
| --------------- | --------------------------------- |
| Juan Becerra    | Car and Game Mechanics Programmer |
| Nate St. George | Car and Game Mechanics Programmer |
| Olivia Thomas   | Car and Game Mechanics Programmer |
| Jared White     | Car and Game Mechanics Programmer |
| Mikey Krentz    | Mapping and AI Programmer         |
| Jared Lytle     | Mapping and AI Programmer         |
| Connor Wood     | Mapping and AI Programmer         |

## Abstract: 
In 1996, Viacom released DeathDrome for Windows 95. DeathDrome was a third-person shooter game consisting of vehicular combat. Vehicles with unique attributes and various weapons would compete against each other, using power-ups scattered across the map to gain advantage over their opponents. Our sponsor, Eric Henderson, being a big fan of the game, wanted it to be remade to rekindle great memories.

In the Fall 2021 semester, Boise State students created BroncoDrome in an attempt to realize Eric’s vision. The game was made using the Unreal Engine for a modern feel. However, a single semester did not prove to be enough time to remake the classic game from scratch. At the end of the semester the game was very barebones with a nonthreatening AI opponent, no way of winning or losing, uninspired models, and nonfunctional power-ups. This was a big problem for Eric who was expecting a fully-fledged game.

With this in mind, Team Gregg took up the baton in an attempt to make BroncoDrome become our sponsor's dream game. Our goal was to supply a finished (yet updatable) game. We set out to to implement most (if not all) of the missing features. In addition to adding these new features, we also intended to improve the pre-existing features, such as the game loop, the menus, the music, and the AI opponent. We committed ourselves to realizing our sponsor’s dream.

## Our finished product:

### Updated Main Menu

### Updated Map Layout

### Updated Music

### Updated HUD

### Updated Models

### Dynamic Soundtrack Added


! Below this point is stuff from the previous semester that will be deleted when we have a finished index.md !

BroncoDrome is an attempt to break the mould of a typical senior design project. 

Requiring not only what is learned in core-classes, but also elective knowledge such as object-oriented design patterns, computer graphics, game creation, classical artificial intelligence, and more, the students working on this project were able to bring their individual talents to create a project that is greater than the sum of its parts.

The goal of BroncoDrome was not to create a complete game, but to create a foundation for future students to build upon and polish something that may be made available to all Boise State students. The objective of the game at this point is to be able to drive the car around the map, fire projectiles, utilize pickups, and interact with AI vehicles.

Built on Unreal's Chaos Engine, this project combines the Unreal Editor, Blueprint files, and C++ code. Due to the early nature of development using these tools, gameplay is not complete, and still open to design changes that could completely change the direction of the game. The game was written in such a way that changing the behavior of the game can happen not only through code, but also the environment. The existing code will work within any type of designed game environment thanks to the development practices used.

Overall, this project was a challenge of not only what we have learned technically, but also problem decomposition and critical thinking. 

## How to Play 
Currently players can drive around the map and shoot enemy cars to accumulate points. The game can be played with either a keyboard or a gamepad. The controls are listed below:  
  
  Move: WASD / left thumbstick    
  Jump: Space bar / face button bottom  
  Shoot: Left mouse button / face button left   
  Target enemy: Tab / right thumbstick button  
  Rotate Camera: Q and E / right thumbstick   
  Look Behind: Left ctrl / right thumbstick   
  Pause: P / special right   

## Development Process
Using Unreal Engine 4.26chaos (for chaos vehicle physics), C++, and blueprints we were able to create a base game that can be taken in any direction from here.

Development started around the idea of building a working vehicle, and world state observers so AI vehicles could be aware of the environment around them.

The vehicle was originally built using the 4.26 Engine physics, not Chaos. This became a big performance problem and posed lots of problems for individual team members; however this would later change. As for the state observation, it was relatively simple to be able to observe where vehicles were, thier general direction, and speed using simple math and global state provided by the Engine.

After these initial steps, the car and map were refined and the AI and pickups were born. Switching the vehicle to use the Chaos engine made development and performance much better overall; combining this with the improved collision map for the existing world, the game began to look like a real game. The addition of timed, generic, and reusable pickups gave the map life and introduced a gameplay mechanic that made driving the car feel more meaningful. The early state of the AI was simple: follow the player and drive around them using a simple attraction and repulsion potential field with agnostic turn direction. Once we hit this point in the project, the next steps were clear: more varied gameplay, player weapons, improved AI, and a better HUD/menu.

Adding variety to the gameplay was something that would take time, as there were plenty of features to add to made the game more complete. These include obstacles for the player to drive around and interact with, as well as weather & times of day. We also improved the HUD and menu system for better player control, and added player weapons with a host of smart functions such as automatic enemy targeting. The weaponry purposefully avoided hitscan detection for the sake of more fun gameplay when more players are added and when AI can use weaponry. The AI had to take on a new approach of potential fields (using active scanning techniques for observation, instead of static observers) to be able to detect obstacles, players, and pickups. Unfortunately, this approach lends itself better to a more open enviornment, and may be better to be reworked again; perhaps a different approach here will be better.

Overall the state of the game is incomplete, but we feel as though we fulfilled more than what we had promised in terms of ability, gameplay, and project contribution.

Quick CI/CD Note: Due to the complex nature of this project, Github Actions could not be used for CI/CD. For more information regarding the CI/CD/devsecops processes and future work regarding this see our directory: https://github.com/juanbecerra0/BroncoDrome/tree/main/CICD  
  
## Screenshots
![Screenshot1](Picture1.png)  
![Screenshot2](Picture2.png)  
