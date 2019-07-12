#!/bin/sh
# -------------------------------------------------------------------------------
# Decimal Format Test
echo "Generating header from testapp using decimal output format"
../bin2c/bin2c ../testapp/testapp output.h TESTAPP
echo "Transforming header back into a binary testapp"
../c2bin/c2bin output.h testapp.generated
echo "Comparing the output of original and generated binaries"
chmod +x ../testapp/testapp
chmod +x testapp.generated
../testapp/testapp > testapp.output.txt
./testapp.generated > testapp.generated.output.txt

diff -s testapp.output.txt testapp.generated.output.txt

rm -f testapp.generated testapp.output.txt testapp.generated.output.txt output.h
# -------------------------------------------------------------------------------
# Hexadecimal Format Test
echo "Generating header from testapp using hexadecimal output format"
../bin2c/bin2c ../testapp/testapp output.h TESTAPP
echo "Transforming header back into a binary testapp"
../c2bin/c2bin output.h testapp.generated
echo "Comparing the output of original and generated binaries"
chmod +x ../testapp/testapp
chmod +x testapp.generated
../testapp/testapp > testapp.output.txt
./testapp.generated > testapp.generated.output.txt

diff -s testapp.output.txt testapp.generated.output.txt

rm -f testapp.generated testapp.output.txt testapp.generated.output.txt output.h