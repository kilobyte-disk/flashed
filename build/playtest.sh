#!/bin/bash

# kilobyte-disk 2025

# Playtester script.

# Configuration
PACKAGE_DIRECTORY="flashed-main"
EXECUTABLE="flashed"

# Sanity check
if [ ! -d $PACKAGE_DIRECTORY ]; then
	echo "ERROR: ${PACKAGE_DIRECTORY} does NOT exist!"
	exit
fi

cd $PACKAGE_DIRECTORY

# Check again
if [ ! -x $EXECUTABLE ]; then
	echo "ERROR: ${EXECUTABLE} does NOT exist!"
	exit
fi

./$EXECUTABLE
