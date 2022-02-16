# BroncoDrome  
![BroncoDrome](https://github.com/cs481-ekh/s22-gregg/actions/workflows/main.yml/badge.svg)  

BroncoDrome is a 3rd person vehicular combat game being developed using Unreal Engine 4. This project was created to satisfy the requirements of CS 481 - Senior Design, but it will likely be the start of a multi-semester project that new groups will incrementally take over. This project will be an opportunity for CS students who are interested in game design careers to work on a relevant project for their capstone. 

## Contributors Fall 2021

| Name            | Role                              |
| --------------- | --------------------------------- |
| Juan Becerra    | Car and Game Mechanics Programmer |
| Nate St. George | Car and Game Mechanics Programmer |
| Olivia Thomas   | Car and Game Mechanics Programmer |
| Jared White     | Car and Game Mechanics Programmer |
| Mikey Krentz    | Mapping and AI Programmer         |
| Jared Lytle     | Mapping and AI Programmer         |
| Connor Wood     | Mapping and AI Programmer         |
| Joey Sacino     | Texture and UI Artist             |
| Tj Davis        | Audio Producer                    |

## Contributors Spring 2022

| Name             | Role                              |
| ---------------- | --------------------------------- |
| Doug McEwen      | Audio Programmer / Nerd Wrangler  |
| Ben Bettencourt  | Game Loop / Mechanics Programmer  |
| DJ Roychoudhury  | Game Loop / Mechanics Programmer  |
| Ross Rippee      | Game Loop / Mechanics Programmer  |
| Saulo Gragg      | Programmer / Modeler              |
| Joseph Kitzhaber | Programmer                        |
| Marie Phelan     | AI Programmer                     |


## Project Setup

### Development Environment

BroncoDrome is developed using [Unreal 4.26.0](https://www.unrealengine.com/en-US/download). For developing in C++, [Visual Studio 2019](https://visualstudio.microsoft.com/downloads/) is highly recommended along with the ‘Game development with C++’ workload as Unreal can automatically generate VS Solutions and hot-load compiled code straight from the IDE. For more info, [check out this article](https://docs.unrealengine.com/en-US/ProductionPipelines/DevelopmentSetup/VisualStudioSetup/index.html).

### Git LFS and Unreal/Git Integration

This repository uses Git LFS, which means that large binary assets are stored in a separate location from this repository. If assets don’t seem to show up in your editor, try entering the following command in the root directory to pull these assets:
```
git lfs pull
```

Be sure to enable Git Source Control within the project editor. To do so:
- Select 'Source Control' in the main Unreal Window (next to the 'Save' icon in the top-left corner)
- Select 'Change Source Control Settings'
- Select 'Git (beta version)' as the Provider
- Provide a path to git.exe if it is not already filled
- Provide your GitHub login credentials if prompted
- Click 'Accept Settings'

Now, most assets (namely Blueprint assets) will have small icons indicating their change status (pending local commit, changed by another user, etc.). Additionally, you can right-click on these assets to view source control history or to open up Git Diffing / Merging tools.

### Building

- Download the Epic Games Launcher: https://www.epicgames.com/store/en-US/download
- Download Unreal Engine: https://www.unrealengine.com/en-US/
-- ENSURE THAT YOUR VERSION IS 4.26chaos
- Clone this repository
- Launch the Unreal Engine from the Epic Games Launcher
- Open BrocncoDrome.uproject in the UnrealEditor
- In the top of your editor you will see a build menu with many choices for Building, Compiling, Testing, etc. These can build installers for specific systems; however this build process can still be automated, see our CI/CD directory in the repository for more information.
