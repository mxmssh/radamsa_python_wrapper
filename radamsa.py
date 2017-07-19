import os
import sys
import ctypes
from ctypes import *

if len(sys.argv) < 2:
    print "Usage: radamsa.py <string_to_mutate>"
    sys.exit(1)

test = ctypes.cdll.LoadLibrary("./radamsa.so")
print test
test.restype = c_char_p
result = test.Mutate(sys.argv[1])
print hex(result)
