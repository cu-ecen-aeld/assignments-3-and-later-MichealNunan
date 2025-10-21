#!/bin/sh

	# Check that we have 1 arguments after the script
if [ $# -ne 2 ];then
	echo "ERROR. Invalid number of arguments"
	echo "There should be 2 argument" 
	echo " 1)The directory and filename text string" 
	echo " 2)The text to insert in the file"
	exit 1

fi

dirAndFile=$1
echo " The directory and file string is:  "${dirAndFile}""

theText=$2
echo " The text to insert into the file is:  "${theText}""

fileName=$(basename "$dirAndFile")

echo "file name is $fileName"

dirName=$(dirname "$dirAndFile")

echo "directory name is $dirName"

	# Check that the directory actually exists if it doesnt create it
if [ ! -d "$dirName" ];then
	echo "The directory does not exist so let's create it"
	mkdir -p "$dirName"
fi

	# Create file using echo cmd
	
echo "$theText" > "$dirAndFile"


if [ ! -e "$dirAndFile" ];then
	echo "ERROR . The file creation didnt work"
	echo ""$dirAndFile" doesnt exist"
	exit 1
fi

