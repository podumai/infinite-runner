#!/bin/bash

declare -i currentStep=1
declare -i totalSteps=6
buildFolderSuffixes=""

for arg in "$@";
do
  if [ $arg == "--debug" ]; then
    totalSteps=10
    buildFolderSuffixes="debug"
    break;
  fi
done

buildFolderSuffixes+=" release"

echo "|************************INFINITE-RUNNER-BUILDER************************|"
echo "[$currentStep/$totalSteps] Checking for presence:"
echo "      mkdir -> Creating build directories;"
echo "      mv -> Moving contents;"
echo "      rm -> Removing temporary files;"
echo "      cd -> Moving between folders;"
echo "      pwd -> Showing working directory."

lineDelimiter="|***********************************************************************|"

commands="mkdir mv rm cd pwd"
declare -i helperStep=1
declare -i isValidStep=0

for command in $commands;
do
  if command -v $command &> /dev/null; then
    echo "[$currentStep.$helperStep/$totalSteps] Check $command command: STATUS [SUCCESS]"
  else
    isValidStep=1
    echo "[$currentStep.$helperStep/$totalSteps] Check $command command: STATUS [FAILED]"
  fi
  helperStep+=1
done

echo $lineDelimiter

if [ $isValidStep -ne 0 ]; then
  echo "[MESSAGE] Error in build process detected"
  echo "[ERROR] One of the command requirements not satisfied"
  echo "[MESSAGE] Terminating process..."
  echo $lineDelimiter
  exit 1
fi

currentStep+=1

echo "[$currentStep/$totalSteps] Start making folders to build game executables."

helperStep=1

for buildFolderSuffix in $buildFolderSuffixes;
do
  mkdir "build_$buildFolderSuffix" &> /dev/null
  if [ $? -eq 0 ]; then
    echo "[$currentStep.$helperStep/$totalSteps] Folder build_$buildFolderSuffix: STATUS [CREATED]"
  elif [ $(ls | grep build_ | wc -w) -ne 0 ]; then
    echo "[$currentStep.$helperStep/$totalSteps] Folder build_$buildFolderSuffix: STATUS [EXISTS]"
  else
    isValidStep=1
    echo "[$currentStep.$helperStep/$totalSteps] Folder build_$buildFolderSuffix: STATUS [CREATION FAILED]"
  fi
  helperStep+=1
done

echo $lineDelimiter

if [ $isValidStep -ne 0 ]; then
  echo "[MESSAGE] Error in build process detected"
  echo "[ERROR] Unable to create folders for game files"
  echo "[MESSAGE] Terminating process..."
  echo $lineDelimiter
  exit 1
fi

if ! command cmake &> /dev/null; then
  echo "[MESSAGE] Error in build process detected"
  echo "[ERROR] Unable to execute cmake"
  echo "[MESSAGE] Try installing cmake to suppress this error"
  echo "[MESSAGE] Termintaing process..."
  echo $lineDelimiter
  exit 1
fi

helperStep=1

for buildFolderSuffix in $buildFolderSuffixes;
do
  currentStep+=1
  echo "[$currentStep/$totalSteps] Moving from $(pwd) -> $(pwd)/build_$buildFolderSuffix"
  cd "build_$buildFolderSuffix" &> /dev/null
  currentStep+=1
  echo "[$currentStep/$totalSteps] Start building $buildFolderSuffix version"
  cmakeResultStr=$(cmake -DCMAKE_BUILD_TYPE="${buildFolderSuffix^}" ..)
  if [ $? -ne 0 ]; then
    echo $cmakeResultStr
    echo "[MESSAGE] Error in build process detected"
    echo "[ERROR] CMake configuring error"
    echo "[MESSAGE] Build: ${buildFolderSuffix^}"
    echo "[MESSAGE] Terminating process..."
    echo $lineDelimiter
    exit 1
  fi
  currentStep+=1
  movingPath="[$currentStep/$totalSteps] Moving from $(pwd) -> "
  cd .. &> /dev/null
  movingPath+=$(pwd)
  echo $movingPath
  cmakeBuildResult=$(cmake --build "build_$buildFolderSuffix")
  if [ $? -ne 0 ]; then
    echo $cmakeBuildResult
    echo "[MESSAGE] Error in build process detected"
    echo "[ERROR] CMake build error"
    echo "[MESSAGE] Build: ${buildFolderSuffix^}"
    echo "[MESSAGE] Terminating process..."
    echo $lineDelimiter
    exit 1
  fi
  currentStep+=1
  echo "[$currentStep/$totalSteps] End building $buildFolderSuffix version"
done

echo $lineDelimiter
echo " No errors detected"
echo " Successfully build the project"
echo $lineDelimiter
