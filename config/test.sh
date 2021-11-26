#!/bin/bash

testApp="../build/apps/test_app"
broker="../build/apps/broker"

read -p "Enter test number (1 to 3): " t

if test $t -eq 1
then
	$testApp t1n1.txt 1 & $testApp t1n2.txt 2 & $testApp t1n3.txt 3 
else 
    if test $t -eq 2
    then
	    $testApp t2n1.txt 1 & $testApp t2n2.txt 2
    else 
        if test $t -eq 3
        then 
            $testApp t3n1.txt 1 & $testApp t3n2.txt 2 & $testApp t3n2.txt 3 
    fi
fi
