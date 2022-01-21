#!/bin/bash

EXIT_CODE=0

echo "REMOVING OLD BRONCODROME ARTIFACTS"

rm -rf /var/unreal_home/BroncoDrome/*

echo "ARTIFACTS REMOVED"
echo "GENERATING MAKEFILE"

cp -r ./* /var/unreal_home/BroncoDrome/
cd /var/unreal_home
./GenerateProjectFiles.sh

echo "MAKEFILE GENERATED... RUNNING"

make -j6 BroncoDrome-Linux-Debug

echo "SUCCESS. MOVING ON TO TESTING"

exit $EXIT_CODE
