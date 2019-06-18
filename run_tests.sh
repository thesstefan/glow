#!bin/bash

echo "*** Running Tests ***"
cd test

for test in *; do
    if [ -d $test ]; then
        cd $test 

        cp ../../Makefile .
        cp ../../test_config.make config.make
        cp ../../test_addons.make addons.make

        make Debug

        cd ..
    fi
done

for test in *; do
    if [ -d $test ]; then
        cd $test/bin

        bin=$(basename ${test})

        ./${bin}_debug

        error_code=$?

        if [[ $error_code -ne 0 ]]; then
            exit $error_code
        fi

        cd ../..
    fi
done

echo "ALL TESTS PASSED SUCCESSFULLY"
