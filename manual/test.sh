#!/bin/bash

path=$1
echo path = "${path}"

filename=$(basename "${path}")
echo filename = "$filename"

suffix=${filename##*.}
echo suffix = "$suffix"

#rootname=$(basename -s ${suffix} ${filename}) ${suffix}
echo rootname = "$rootname"

#filename=$(basename -az "$1")
#filename=$(basename) /home/tim/devel/pwbeaver/test.sh

#/path to directory/root name/new suffix
