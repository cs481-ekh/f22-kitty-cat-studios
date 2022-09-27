# AI Behavior

## Observations

AI doesn't seek out player car or run away. Most of the AI cars are spinning in circles,
shooting aimlessly until the player gets within it's radius. From then, the car will shoot
at the player but the movement is still usually in the circular motion.

AI cars will shoot at each other if in each others radius but I don't believe that is the
intended behavior as I believe it's the AI cars against the player car. The player can
win a round by not even moving if the AI cars spawn close enough together due to this
mechanic.

AIs shooting is accurate on the player if the player is not moving quickly past the enemy
car. If the player car is moving at its max velocity, it seems the AI can't keep up with
its targeting. While consistent, it's not good for even a beginner level AI.

## Suggestions

* Have enemy AI stop targetting each other
* Get AI cars to path around the area, either running away from the player or seeking the player out
* Fix collision collider not de-spawning when AI car is destroyed

## States

- Fighting
The enemy runner should chase down the player while firing.

- Fleeing
The enemy runner should continually put more distance between themselves and the player in this state.

```
                           Enemy runner
                               NOT
                           taking┼damage
                           │           │
                           │           │
                       ┌───▼───┐       │
                       │       ├───────┘
                       │       │
                       │Fighting
  No buttons           │       │
┌───pressed───┐        │       ├──Quit─────┐
│             │        └───▲──┬┘  game     │
│         ┌───▼───┐        │  │        ┌───▼───┐
│         │       │        │  │        │       │
│         │ Main  │ Game   │  │        │ End   │
└─────────┤ Menu  ├─start──┘  │        │ Game  │
          │       │           │        │       │
          │       │       Enemy runner │       │
          └───────┘       taking damage└───▲───┘
                           │               │
                       ┌───▼───┐           │
                       │       │           │
                       │       ├──Quit─────┘
                       │Fleeing│  game
                       │       │
                       │       ├───────┐
                       └───▲───┘       │
                           │           │
                           │Enemy runner
                           │taking┼damage
```