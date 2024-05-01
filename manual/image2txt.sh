#!/bin/bash

#https://www.thesitewizard.com/html-tutorial/embed-images-with-data-urls.shtml

if [ $# -eq 0 ]
    then
    echo "Usage: image2text pngFile\n"
    exit 1
fi

# create input file path with suffix stripped but directories in tact
rootname=${1}

# create output file name
txtfile=${rootname%.*}.txt

echo Reading ${rootname}, writing ${txtfile}

# do the conversion
base64 ${rootname} > ${txtfile}

echo Done
