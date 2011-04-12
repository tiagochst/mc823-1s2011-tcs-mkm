#!/bin/bash

for((i=0;i<100;i++))
do
    ./c guns < ./entradas/input1
    mv asd asd~
done

mv *.dat 1

for((i=0;i<100;i++))
do
    cp asd~ asd
    ./c guns < ./entradas/input2
done

cp asd~ asd

mv *.dat 2

for((i=0;i<100;i++))
do
    ./c guns < ./entradas/input3
done

mv *.dat 3


for((i=0;i<100;i++))
do
    ./c guns < ./entradas/input4
done

mv *.dat 4


for((i=0;i<100;i++))
do
    ./c guns < ./entradas/input5
done

mv *.dat 5
