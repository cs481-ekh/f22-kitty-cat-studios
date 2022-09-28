# Existing CI

Currently, the CI pipeline has two tasks. One attempts to build an executable
for linux. The other runs a linter on the entire project. The comments in
`.github/workflows/main.yml` describe this accurately.

## Linux Build

The Linux build uses a third-party docker container to provide Unreal Engine 4.
Our source code is mounted in the container and the `build.sh` script is
executed.

## Windows Build

Building for Windows automatically is currently disabled. The overall process is
similar to the Linux build, but the build script `build.cmd` is used.

## Super-Linter

Super-Linter is a GitHub provided Action that combines many existing linter
tools. It appears to lint our C++ code using `cpp-lint` and `clang-format`. Lints are also applied to plain text files like this one.
