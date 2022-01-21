# UBUNTU JENKINS AGENT

We needed a jenkins agent that we can spin up dynamically and build from; however, there weren't any images provided that could do this, so looking at the alpine-based docker-agent image I was able to determine how to build our own agent that we may have to maintain in the future. This is a fantastic project for future students, as it will force them into farmiliarity with Docker and Jenkins. With only a little bit on knowledge of each tool, you can shrink this image to be smaller and more light-weight, but we didn't have that kind of time.

The idea behind this agent was to make a re-usable agent that can be a base-image for other images to be built on top of.

Other than being a bit bloated, this agent is for all intents and purposes "complete".
