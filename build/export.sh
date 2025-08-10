#!/bin/bash

# kilobyte-disk 2025

# Export the project into a
# package that can be easily
# distrubuted.

# CONFIGURATION:
FOLDER_NAME="flashed-main"
EXECUTABLE_NAME="flashed"

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
SRC_ASSETSDIR="../assets"
if [ ! -d "$SRC_ASSETSDIR" ]; then
	echo "ERROR: Unable to locate ${SRC_ASSETSDIR}"
	exit
fi

SRC_DATADIR="../data"
if [ ! -d "$SRC_DATADIR" ]; then
	echo "ERROR: Unable to locate ${SRC_DATADIR}"
	exit
fi

echo "${SRC_ASSETSDIR}"
cp -r $SRC_ASSETSDIR "${PKGDIR}/"
echo "${SRC_DATADIR}"
cp -r $SRC_DATADIR "${PKGDIR}/"

# Move the executable file to the package directory
echo "Moving executable..."
if [ ! -x "$EXEDIR" ]; then
	echo "ERROR: Unable to locate executable ./${EXEDIR} !"
	exit
fi

mv $EXEDIR "${PKGDIR}/"

echo "Successfully generated ${FOLDER_NAME} package!"
