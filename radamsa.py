import os
import sys
import ctypes
from ctypes import *

if len(sys.argv) < 2:
    print "Usage: radamsa.py <string_to_mutate>"
    sys.exit(1)

test = ctypes.cdll.LoadLibrary("./radamsa.so")
print test
res_str = create_string_buffer('\0', 3000)
result = test.Mutate(sys.argv[1], byref(res_str))
result_string = cast(res_str, c_char_p).value
print "Mutate returns %d:" % result
print "Mutated string %s:" % result_string
