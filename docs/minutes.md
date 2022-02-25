Minutes of the team meetings go here:

## Whole Team Meeting 2-18 (Week 6)
Sprint: 1  
Present: Ben, Doug, DJ, Joey, Marie, Saulo, Ross  
Absent: [N/A]  
Scribe: DJ  
  
Scrum Master: Joey  
Product Owner: Ross  
Devs: Ben, Doug, DJ, Marie, Saulo  
Time: 1330-1405 (35 minutes)    


*Sprint 0 Recap:
- Ross: if runner gets to zero health, it dies
- Ben: CICD debugging, Linux build working, comment Windows for now, added linter and docs to main.yml
- Marie: backwards movement added
- DJ: AI can shoot bullets 
- Doug: codebase research - sound
- Saulo: modeling of blaster turret
- Joey: codebase research - win conditions  


*Sprint 1 Update:
- Marie: Movement decisions
- DJ: locate player, aim target laser
- Ross: on death- teleports to specific location, restores health
- Ben: CICD debugging- Linux build working, comment out Windows for now, added linter and docs to main.yml
- Doug: dynamic soundtrack, research on implementation
- Saulo: refactoring powerup function
- Joey: game stops when condition reached, working on high score screen/widget with Ben  


*Sprint 1 ToDo:
- Joey: win conditions
- DJ: shoot at player
- Marie: movement decisions
- Saulo: powerup master class, children with different attributes
- Ross: delay in respawn, random locations
- Ben: high score and win conditions
- Doug: implement dynamic sounds  


Suggestions:
- modify shots so that ricocheted bullets do not damage the runner that shot it.
- 'tweaks' channel/tasks for bugs and issues that can be solved when anyone gets a chance
- have the AI spawn at a fixed location for ease of testing
- useful resources in google doc in shared folder  

## Whole Team Meeting 2-11 (Week 5)
Present: Everyone
Absent:
Scribe: Ross

Meeting started at 1:33pm on Friday, February 11th

Joey will be Scrum Master

Ross will be Product Owner

DJ will be Scribe

Ben, Doug, Marie, and Saulo are Devs

These roles will last until Friday, February 18th. 

This is our official Scrum planning meeting

Sprint Goal: have a deliverable version of the game containing whatever features were added during this sprint.

Adding a linter for the C++ will be adequate for the CI/CD setup.

Definition of Done: Everyone is responsible for making a new
branch when starting work on a task. Everyone submits pull requests when they are
ready to merge their branch into master. Two people must review the request. Priority will be put on getting
other members of your section to review your work before other members of the group. The code should be
adequately commented according to the group's agreed-upon commenting structure.

Because we have members from multiple sections, we will have our sprints end on Fridays instead of Tuesdays to
accomodate not being able to all meet together on Tuesdays.

Marie suggested that instead of having our own branches, we make branches for every task.

Doug's goal: get the music assets into the game, even if they are never referenced by the code

Ross's goal: player death and respawn

Joey's goal: when you hit a certain amount of points, the game stops, you're prompted that you won, and are
asked if you want to start over

DJ's goal: the AI can aim at the player and shoot

Marie's goal: the AI can move around differently

Ben's goal: the main menu works (accomplished), implement C++ linter, stretch goal of displaying high-score

Saulo's goal: implement something similar to a power-up that increases damage, & update the models and textures 

Sprint retrospective planned on the Fridays.

Meeting ended at 2:05

## Afternoon Meeting 2/8 (Week 5)

Present: Marie, DJ
Absent: 
Scribe: Marie

~45 minutes

Broke down the AI project road map. Decided what will be done in each sprint for AI. 

## Whole Team Meeting 2/4 (Week 4)

Present: Everyone
Absent: No one
Scribe: Ross/Marie

~2 hours

We spent about 45 minutes troubleshooting environment issues and ensuring that everyone was set up for sprint 0.

The rest of the meeting was spent going through the project plan section by section to ensure that it would be ready by the due date. 

## Morning meeting 2/3 (Week 4)
Present: Joey, Saulo, Ben, Doug, Ross
Absent: DJ, Marie
Scribe: Ross

~20 minutes

Discussed the policy of having individual branches
Discussed the structure of the github's Project layout
Decided that tomorrow's meeting will focus on making sure everyone has a working, testable version of the game. We will help anyone
still having issues. We will also decide on the new roles.

## Morning meeting 2/1 (Week 4)

Present: Joey, Saulo, Ben, Doug, Ross
Absent: DJ, Marie
Scribe: Ross

~15 minutes

Discussed what was left to do for the project setup:
  - Make sure everyone signs and submits the Team Contract
  - Make sure everyone is a contributor on the repository
We are going to reuse the previously made license
Saulo may take responsibility for designing the second level if the powerups don't take too long

## Sponsor meeting 1/29 (Week 3)

Present: All
Scribe: Ross

Discussed the project's requirements:
  - Developer requirements (noted as not important to the sponsor)
  - Working source code (Upon realizing there were still issues with the provided source code, the sponsor agreed to make     an effort to provide a functional version of the old source code [now accomplished])
Asked questions about sponsor's expectations:
  - The previous team started setting up Jenkins and Docker, should we work on integrating them into our project?
    - The sponsor explained that it would be very difficult to use CI/CD with this project, so it is not a requirement
  - Do we need to make a Mac-compatible release of the game?
    - The sponsor will not require that
  - What should we do if we can't get the old code to work?
    - The sponsor responded by asking if we wanted to start from scratch
      - We do not want to start from scratch, but if there is an issue with an aspect of the old code that we 
      can't figure out, we may choose to re-write that aspect from scratch
  - What does the sponsor want to see in the game?
    - Required: Scoring, game modes, smarter AI, power-ups, distinct AI (different attributes for different AI affecting
    their performance in the game as well as unique models), explosion effect on destruction [may be a stretch goal]
    - Stretch goals: Another level, dynamic soundtrack, local high-score system
  - Milestones:
    * Milestone 1 is to get the game testable for everyone on the team
    * Milestone 2 is to split up into groups adding required features of the game (sponsor expects this to be finished
    before the midpoint of the semester)
    * Milestone 3 is to merge all the new features of the game into a playable version of the game
    * Milestone 4 is to add stretch goals while continuing to have a playable version of the game
  - Contingencies:
    - If milestone one takes too long, we will have to cut planned features to adjust for that.
    - If portions of the existing code can't be fixed they will have to be replaced with new code from scratch, which
    could take enough time to have to cut planned features of the game
  - Communicataion:
    - No regularly scheduled sponsor meetings since our sponsor happens to see us in class, but we are encouraged to
    ask to schedule sponsor meetings as needed
    - Rather than waiting for a collection of issues that require our sponsor's attention to send our sponsor an email,
    we are encouraged to send emails regarding problems we are facing as they arrive (or discuss during class)
  - Deliverables:
    - Just an executable version of the (playable) game [sponsor only, our teacher also wants our source code] by 
    April 27

## Team meeting 1/28 (Week 3)

Present: Joey, Saulo, Ben, DJ, Marie, Doug
Absent: Ross
Scribe: Joey

Talked about using Empty Actors for spawning.
Issue may be not spawners, but may be Players/Enemies. Reason? We can't find a blueprint for players/enemies.
Some of us still having building issues.
Found C++ file for menu hud
Discussed Sponsor meeting agenda and filled out our "script"



## Morning meeting 1/27 (Week 3)

~10 minutes

Ben, Ross, Saulo, Doug

Scribe: Joey

Planning for sponsor meeting
M1: get it running
M2: add xyz feature
M3: having tasks completed
M4: have the full project completed (refinement)
People in similar work areas should keep an eye on each other.
May have to drop some features to work on other, more important details. 