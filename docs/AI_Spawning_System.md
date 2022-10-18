# AI Spawning

## Overview

In Fall 2022, the spawning system for BroncoDrome was overhauled. Previously, spawning would pick a random point on the map in the air and would not account for nearby runners or obstacles. In addition, AI "respawned" like the player when they died and had a limited number of lives. Ultimately a new system would need to be implemented and that was one of the team member's goals for sprint 1 and 2. In the new system, there is a single AI Spawner "Controller" which will control individual AI Spawner points. Instead of complete randomness, spawning now happens at AI Spawner points and is based on robust configuration parameters in the AI Spawner controller. This ultimately allows for a much more dynamic spawning system that will enable more interesting gameplay and the possibility of new gamemodes.

## Spawn Controller

The relevant controller files are located at 
```
/Source/BroncoDrome/StageActors/AISpawnerController.cpp and /Source/BroncoDrome/StageActors/AISpawnerController.h
```

In essence, the spawner controller will grab and store all other spawner objects on the map as well as periodically keep track of the current number of active runners. It will use this information and its configured parameters to call spawn methods on the AISpawners and respawn AI at the available spawn points.

In game, you can search for an actor object **MyAISpawnerController** which is a child of AISpawnerController.cpp and can be placed anywhere on a map. There must be at least one and ONLY one spawner controller per map. The spawner controller has multiple parameters that are fairly self-explanatory (they have tooltips as well) and can be configured on demand in the UE map editor through the actor object.

## Spawner

The relevant spawner files are located at 
```
/Source/BroncoDrome/StageActors/AISpawner.cpp and /Source/BroncoDrome/StageActors/AISpawner.h
```

These spawner points are designed to be easy for map creators to place and shift around. They have a couple of unique properties for debugging, but in general their default values are preferred and only their location should be modified.

In game, you can search for an actor object **MyAISpawner** which is a child of AISpawner.cpp and can be placed wherever you want a spawn point to be. You must place at least one, but can place as many as you would like. It is recommended that you place at least as many spawn points as the configured spawner controller "MaxAI" value so that at game start all the AI will spawn.
