# Perform the build in an Unreal Engine development container image that includes the Engine Tools
FROM adamrehn/ue4-full:4.26-chaos AS builder

# Clone the source code for our Unreal project
RUN git clone --progress --depth 1 https://github.com/cs481-ekh/f22-kitty-cat-studios.git /tmp/project

# Build and package our Unreal project
# (We're using ue4cli for brevity here, but we could just as easily be calling RunUAT directly)
WORKDIR /tmp/project
RUN ue4 package