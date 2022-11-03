#!/usr/bin/env bash

# first, find root directory
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
ASSIGNMENT_DIR=$(dirname "$SCRIPT_DIR")

cd $ASSIGNMENT_DIR
mkdir -p data
cd data
curl --remote-name https://www.sfelections.org/results/20161108/data/20161206/20161206_ballotimage.txt
curl --remote-name https://www.sfelections.org/results/20161108/data/20161206/20161206_masterlookup.txt