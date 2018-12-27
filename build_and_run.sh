#!/bin/bash

# Set basic variables:
# Command name.
mode=$1

target_folder_name="target"

# Create target folder.
if [ ! -d ${target_folder_name} ]; then
    mkdir ${target_folder_name}
fi

echo -e "###########################################\n"

OPTIONS=""
# Execute commands:
if [ "$mode" == "seqn" ]; then

    echo "## Sequential mode."
    echo "## - drawing is disabled"
	echo -e "\n###########################################"

    OPTIONS+="-DOPT_SEQN=ON"

elif [ "$mode" == "openmp" ]; then

    echo "## Parallel mode using OpenMP."
    echo "## - drawing is disabled"
	echo -e "\n###########################################"

    OPTIONS+="-DOPT_OPEN_MP=ON"

elif [ "$mode" == "mpi_seqn" ]; then

    echo "## Parallel mode using MPI on seqn program."
    echo "## - drawing is disabled"
    echo -e "\n###########################################"

    RUN_WITH_MPI=true
    OPTIONS+="-DOPT_MPI=ON -DOPT_SEQN=ON"

elif [ "$mode" == "mpi_openmp" ]; then

    echo "## Parallel mode using MPI on openmp program."
    echo "## - drawing is disabled"
    echo -e "\n###########################################"

    RUN_WITH_MPI=true
    OPTIONS+="-DOPT_MPI=ON -DOPT_OPEN_MP=ON"

elif [ "$mode" == "clean" ]; then

    echo "cleaning"
	echo -e "\n###########################################"

    rm -r "${target_folder_name}"
    exit 0

elif [ "$mode" == "default" ]; then

    echo "## Default sequential mode."
    echo "## - drawing is enabled"
	echo -e "\n###########################################"

    OPTIONS+="-DOPT_DRAWING=ON"

else

	echo -----------------  HELP  --------------------
	echo -- Chose one from the following command: --
	echo
	echo "seqn              : Sequential mode"
	echo "    Call arguments: seqn [tests number]"
	echo "openmp            : parallelization using OpenMP"
	echo "    Call arguments: openmp [tests number] [OpenMP threads number]"
	echo "mpi_seqn          : parallelization using MPI on seqn program"
	echo "    Call arguments: mpi_seqn [MPI nodes number] [tests number]"
	echo "mpi_openmp        : parallelization using MPI on openmp program"
    echo "    Call arguments: mpi_openmp [MPI nodes number] [tests number] [OpenMP threads number]"
    echo "default           : default mode with drawing graphs"
    echo " "
    echo -e "After mode provide argument for program execution in specific mode\n"
    echo "In case of unexpected behaviour run clean"
	echo -e "\n###########################################"
	exit 1

fi

# Build
cmake ${OPTIONS} . -B"${target_folder_name}"/build
cmake --build "${target_folder_name}"/build

# Execute program
echo -e "\n\nExecute:\n\n"
if [ ${RUN_WITH_MPI} ]; then
    if [ -z "$2" ]; then
        echo -e "Please provide number of processes as argument\n"
        exit 2
    fi

    PROCESS_CNT=$2
    mpirun -np ${PROCESS_CNT} ./target/build/porr_dpso ${@:3}
else
    ./target/build/porr_dpso ${@:2}
fi
