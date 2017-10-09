#!/bin/bash

imgs=`ls *.png`

for file in $imgs
do
	echo $file
	newfile=${file%".png"}-gray.png
	convert $file -fx '(r+g+b)/3' -colorspace Gray $newfile
	echo $newfile
done
