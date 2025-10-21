#!/bin/sh

	# Check that we have 2 arguments after the script
if [ $# -ne 2 ];then
	echo "ERROR. Invalid number of arguments"
	echo "The total number of arguments should be 2"
	echo "The order of the arguments should be:"
	echo " 1)File Directory Path"	
	echo " 2)String to be searched for in the specified directory path"	
	exit 1

fi

filesdir=$1
#echo " The directory:  "${filesdir}""

searchstr=$2
#echo " The string: "${searchstr}""

#echo " "

	# Check that the directory actually exists
if [ ! -d "$filesdir" ];then
	echo "ERROR: The directory does not exist"
	exit 1
fi


	# Count the files in directories and subdirectories
NbOfFiles=$(find "$filesdir" -type f | wc -l)

	# Count the number of times the word occurs across all those files
	# zithout the unreadable files
NbOfLines=$(grep -roh "$searchstr" "$filesdir" 2>/dev/null | wc -l)

#echo "In $filesdir counting '$searchstr'"
#echo "--------------------------------------------"
echo "The number of files are "$NbOfFiles" and the number of matching lines are "$NbOfLines""
