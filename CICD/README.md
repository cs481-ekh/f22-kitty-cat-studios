# CI/CD Overview

## Philosophy and Foreword

This is by far not the most complete CI/CD Implementation possible; however, it is wokring and comfortable enough to trust.

The main idea here was to create a Jenkins environment with two simple jobs: One for merges to develop (Builds our code after a feature branch is merged to develop). The next is to test the ability to build an installer when we merge from develop to master (main).

Conceptually, we wanted the ability to make a jenkins agent docker image that could be spun up at will in the context of a docker cloud, but not have to put the entier UnrealEngine on this agent's image. This was both tricky and fun, especially since building the Engine on our build server was close to impossible with the firewall/security restrictions in place.

Overall, we built this with the concept of creating a future project for another semester to deal with. We laid the foundation of a basic idea, with the ability to make this infinitely scalale if they wish. See 'Future Projects' below for ideas. 

## Requirements

### Build Server Access

To access the build server, you must be on onyx. You will likely want X-forwarding enabled.

The build server can be accessed with:
$ <username>@ubuntu-broncodrome

If you know how, setting up your public ssh key in your .ssh directory in authorized_keys on the server will save you the hassle of entering your password each time.

### Docker

The build server already contains the docker command, and it has been made so that you do not need to use sudo to use docker; however, you must add yourself to the docker group
in order to be able to do this. To be added to this group, as a sudo-enabled user do:

$ usermod -a -G docker <username>

Then have the user exit, and return to the build server. When they return the command:

$ docker image ls

should work without needing sudo.

### Docker-Compose

Docker compose is already added to the server, and is here for consistency of docker commands.

## Included Directories & Purposes

### Docker Jenkins Agent

For more in-depth info see the readme in the containing directory

### Docker Unreal Agent

For more in-depth info see the readme in the containing directory

### Jenkins

For more in-depth info see the readme in the containing directory

## Afterthoughts

Is this the cleanest/best implementation possible? Definitely not, but we still feel proud of what was accomplished here.

In Two weeks we:

Built a Jenkins Master instance with proper security on a build server. 
Installed the necessary tools for operation (docker, docker-compose)
Created our own version of a Jenkins Agent docker image based on an Ubuntu System (instead of alpine, like the commercial image)
Created an Unreal Engine agent that has all the tools needed to build, without installing the Engine on the image itself.
Built the Engine on a restrictive-access server hidden behind a VPN
Mounted the Engine in a non-time-consuming mannerto the Unreal Build image
Created Jenkins jobs that continuously poll for changes on develop and master via SSH.
Created the ability to build and test the UnrealEngine default compiler when we merge to develop.
Created the ability to test the BroncoDrome Installer when we push to master (main).

What we are sad about:

The lack of a docker cloud.. We really came close to a solution for this, but ultimately were okay with a single-agent, and didn't want to waste any more time.

### Future Projects
- Server security testing
- Jenkins security testing
- Docker cloud contianing slimmed versions of the images contained here.
- Self-building docker jenkins agents (if the agent code is changed in a repository, the agent can build a new instance of itself, and add itself to the docker cloud/agent list)
- Windows Inclusion
