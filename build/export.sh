#!/bin/bash

# kilobyte-disk 2025

# Export the project into a
# package that can be easily
# distrubuted.

# CONFIGURATION:
FOLDER_NAME="flashed-main"
EXECUTABLE_NAME="flashed"

# Functions
safe_cpdir () {
	if [ ! -d "$1" ]; then
		echo "ERROR: Unable to locate ${1}"
		exit
	fi
	
	echo ${1}
	cp -r ${1} "${2}/"
}

# Build the executable
echo "Building executable..."
cmake .. && make

EXEDIR=$EXECUTABLE_NAME

# Generate package directory
PKGDIR=$FOLDER_NAME
if [ ! -d "$PKGDIR" ]; then
	echo && echo "Generating package directory..."
	mkdir $PKGDIR
else
	echo && echo "Package directory already exists."
fi

# Newline
echo "Copying directories..."

# Copy directories
safe_cpdir "../assets" $PKGDIR
safe_cpdir "../data" $PKGDIR
safe_cpdir "../decks" $PKGDIR

# Move the executable file to the package directory
echo "Moving executable..."
if [ ! -x "$EXEDIR" ]; then
	echo "ERROR: Unable to locate executable ./${EXEDIR} !"
	exit
fi

mv $EXEDIR "${PKGDIR}/"

echo "Successfully generated ${FOLDER_NAME} package!"
