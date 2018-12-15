#!/bin/bash

# Set basic variables:
# Command name.
input=$1

target_folder_name="target"

# Create basic target folder.
if [ ! -d $target_folder_name ]; then
    mkdir $target_folder_name
fi

# Execute commands:
if [ "$input" == "synch" ]; then

    # Create build and binary files
    cmake -DMODE="SYNCH" -DDRAWING="FALSE" . -B"${target_folder_name}"/build
    cmake --build "${target_folder_name}"/build 

elif [ "$input" == "pararell" ]; then   

    # Create build and binary files
    cmake -DMODE="PARARELL" -DDRAWING="FALSE" . -B"${target_folder_name}"/build
    cmake --build "${target_folder_name}"/build 

elif [ "$input" == "clean" ]; then
    rm -r "${target_folder_name}"/

elif [ "$input" == "help" ]; then

	echo "###########################################"
    echo " "
	echo -----------------  HELP  --------------------
	echo -- Chose one from the following command: --
	echo -------------------------------------------
	echo " --- #synchro      : " 
	echo " --- #pararell     : "
	echo "###########################################"

else
   
    echo "Default synchro."
    # Create build and binary files
    cmake -DMODE="SYNCH" -DDRAWING="TRUE" . -B"${target_folder_name}"/build
    cmake --build "${target_folder_name}"/build
fi
