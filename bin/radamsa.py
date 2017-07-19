import os
import sys
import ctypes
from ctypes import *

test = ctypes.cdll.LoadLibrary("./radamsa.so")
print test
test.restype = c_char_p
result = test.Mutate(b'tr')
print hex(result)
