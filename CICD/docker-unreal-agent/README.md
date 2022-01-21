# DOCKER UNREAL AGENT

The purpose of this agent is to provide the tools necessary to build using Unreal Engine 4.26 without actually adding Unreal to the image. It's important to note the structure of how this image is launched.

In order to avoid setting up a docker cloud on this protected server, we took the route of spinning up a single box in this instance. With docker cloud, we could dynamically spin up as many as we would like; however, we didn't have that kind of time, so we opted for a single build agent by executing the startup.sh command which will connect to Jenkins master as a single-instance agent.

Note the lack of a docker-compose file in this instance. The reason for this was (even when using a tool like composerize) our startup script command would try to run before the box was finished initializing, and thus we would recieve connection errors from the java code in agent.jar. Our solution was to run bash as an entrypoint to this image, and execute our command after the container had spun up. Docker compose is generally a simple tool; however, in this instance it proved to be quite tricky.

To launch this agent: copy/run the command in command.txt
