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
    * This method is checking a timer and the Runner current lock on status to either start a timer to for locking on or start a count down for the remaining time the Runner can be locked on to it's current target. This method may be fine to keep unless we don't want the AI to stop targeting based off of a timer, instead distances and priority targets.

* QueryLockOnDisengage() **Line 237**
    * Just like QueryLockOnEngage(), this method is checking if it should stop targeting the opposing Runner.

* LockOn() **Line 249**
    * This function will check if it's currently locked on to a Runner. If not, it'll attempt to locate the closest Runner and lock on. If it is locked on, it'll stop targetting whichever Runner it was targeting and stop the lock on timer.

* drawTargetLine(FVector location) **Line 265**
    * This might be one of the more confusing methods implemented currently. I believe it's trying to check if the AI Runners raycast is hitting another Runner and if it is, call LockOn(). Might want to add some debugging to see what exactly it's doing in game.

###  AI Movement

* Raycast(FVector to) **Line 74**

* GetDirection() **Line 92**

* MoveDecision(FVector location) **Line 164**
