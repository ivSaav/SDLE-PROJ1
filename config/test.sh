#!/bin/bash

testApp="../build/apps/test_app"
broker="../build/apps/broker"

read -p "Enter test number (1 or 2): " t

if test $t -eq 1
then
	$broker & $testApp t1n1.txt 1 & $testApp t1n2.txt 2 & $testApp t1n3.txt 3 
else 
    if test $t -eq 2
    then
	    $broker & $testApp t2n1.txt 1 & $testApp t2n2.txt 2
    fi
fi