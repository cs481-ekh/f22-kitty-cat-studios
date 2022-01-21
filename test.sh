EXIT_CODE=0

echo "RUNNING TESTS"
cd /var/unreal_home

make -j6 BroncoDrome-Linux-Test

echo "SUCCESS. TESTS COMPLETED."

exit $EXIT_CODE
