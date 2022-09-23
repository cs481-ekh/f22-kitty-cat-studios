# AI Functions

## /Source/BroncoDrome/StageActors/AIActor.cpp

* AIActor.cpp is built off of /Source/BroncoDrome/Runner/Runner.cpp

### AI Shooting

* ShotDecision(FVector location) **Line 281**
    * For now, I believe we should keep this function for testing our tags and to shoot. If we use an if statement to have raycast look for our AIRunner tag, then shoot, that might give us better functionality.

* Fire() **Line 291**
    * This seems to be functional, it’s doing some checks and then spawning the object to try and hit the player or AI runner.

### AI Target Locking

* QueryLockOnEngage() **Line 224**
    * For now, I believe we should keep this function for testing our tags and to shoot. If we use an if statement to have raycast look for our AIRunner tag, then shoot, that might give us better functionality.

* QueryLockOnDisengage() **Line 237**

* LockOn() **Line 249**

* drawTargetLine(FVector location) **Line 265**

###  AI Movement

* Raycast(FVector to) **Line 74**

* GetDirection() **Line 92**

* MoveDecision(FVector location) **Line 164**