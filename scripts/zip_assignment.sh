#!/usr/bin/env bash

usage() {
    cat <<EOM
    Usage:
    $(basename $0) YOUR_NAME
EOM
    exit 0
}

[ -z $1 ] && { usage; }
NAME=$1

# https://stackoverflow.com/a/246128
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
ASSIGNMENT_DIR=$(dirname "$SCRIPT_DIR")

cd $ASSIGNMENT_DIR

if ! command -v clang-format &> /dev/null
then
    tput setaf 1; echo "clang-format not available. Did you format your code already?"; tput sgr 0;
else
    tput setaf 2; echo "Formatting with clang-format"; tput sgr 0;
    clang-format -i assignment*/**
fi

find . -maxdepth 1 -not -name '.*' -not -name build -not -name cmake-build-debug -print | zip -r "${NAME}.zip" -@
