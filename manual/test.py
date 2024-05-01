#!/usr/bin/python3

import sys

print ('Total arguments:', len(sys.argv))

print("Argument values are:")
# Iterate command-line arguments using for loop
for i in sys.argv:
  print(i)
