from ctypes import *

try:
    lib = cdll.LoadLibrary('./main.so')
except Exception as e:
    print("Could not load library:", e)


class lfloat_t(Structure):
    _fields_ = [("exp", c_int), ("sign", c_ubyte, 1), ("len", c_int, 31),
                ("mantissa", c_char * (lib.mantissa_len() + 1))]

a = lfloat_t()
lib.init_lfloat(byref(a))

lib.print_lfloat(a)
lib.printf("\n")
