#!/bin/bash

rm asd

for((i=0;i<1;i++))
do
    ./c localhost < ./entradas/input1
    mv asd asd~
done

mv *.dat 1

for((i=0;i<1;i++))
do
    cp asd~ asd
    ./c localhost < ./entradas/input2
done

cp asd~ asd

mv *.dat 2

for((i=0;i<1;i++))
do
    ./c localhost < ./entradas/input3
done

mv *.dat 3


for((i=0;i<1;i++))
do
    ./c localhost < ./entradas/input4
done

mv *.dat 4


for((i=0;i<1;i++))
do
    ./c localhost < ./entradas/input5
done

mv *.dat 5
