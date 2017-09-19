from ctypes import *
import curses

try:
    lib = cdll.LoadLibrary('./main.so')
except Exception as e:
    print("Could not load library:", e)


class lfloat_t(Structure):
    _fields_ = [("exp", c_int), ("sign", c_ubyte, 1), ("len", c_int, 31),
                ("mantissa", c_char * (lib.mantissa_len() + 1))]

nums = "".join(map(str, range(10)))
int_f = "+" + "_" * 30
float_f = "+_." + "_" * 29 + "E+" + "_" * 5

def main(stdscr):
    #stdscr.clear()
    curses.noecho()
    curses.cbreak()

    curses.use_default_colors()

    main2(stdscr)

    stdscr.refresh()
    stdscr.getkey()

def main2(stdscr):
    stdscr.addstr(0, 0, ("{:^%ds}" % (curses.COLS - 1))
                  .format(" Professional divider"), curses.A_BOLD)
    stdscr.addstr(1, 0, "A: ", curses.A_BOLD)
    #stdscr.addstr(int_f)
    stdscr.addstr(float_f)
    stdscr.addstr(" / ")
    stdscr.addstr(2, 0, "B: ", curses.A_BOLD)
    stdscr.addstr(float_f)
    stdscr.addstr(" = ")
    stdscr.addstr(3, 0, "R: ", curses.A_BOLD)

    stdscr.move(1, 3)
    astr = input_lfloat(stdscr).encode("utf-8")
    stdscr.move(2, 3)
    bstr = input_lfloat(stdscr).encode("utf-8")
    stdscr.move(3, 3)

    a = lfloat_t()
    b = lfloat_t()
    r = lfloat_t()

    result = lib.input_int_lfloat(astr, byref(a))
    if result != 0:
        stdscr.addstr(3, 3, "Error input: %d" % result)
        return

    result = lib.input_lfloat(bstr, byref(b))
    if result != 0:
        stdscr.addstr(3, 3, "Error input: %d" % result)
        return

    result = lib.div_lfloat(a, b, byref(r))
    if result != 0:
        stdscr.addstr(3, 3, "Error div: %d" % result)
        return

    result = lib.check_lfloat(r)
    if result != 0:
        stdscr.addstr(3, 3, "Error check: %d" % result)
        return

    res = create_string_buffer(80)
    lib.format_lfloat(r, res)
    stdscr.addstr(3, 3, cast(res, c_char_p).value)

def get_sign(stdscr):
    while True:
        x = chr(stdscr.getch())
        if x in "+\r\n " + nums:
            if x in nums:
                return "+", x
            return "+",
        elif x in "-":
            return "-",

def input_lfloat(stdscr):
    r = []

    res = get_sign(stdscr)
    r += res
    for n in res:
        stdscr.addch(n)

    i = 0
    while i <= 30:
        x = chr(stdscr.getch())

        if x in nums + ".":
            if x == "." and x in r:
                continue

            if x in nums:
                i += 1

            r.append(x)
            stdscr.addch(r[-1])

        if x in "eE ":
            for _ in range(30 - i - ("." in r)):
                #r.append("0")
                stdscr.addch("0")
            r.append("e")
            stdscr.addch("E")
            break

        if x in "\r\n":
            return "".join(r)

    res = get_sign(stdscr)
    r += res
    for n in res:
        stdscr.addch(n)

    i = 0
    while i <= 5:
        x = chr(stdscr.getch())

        if x in "\r\n ":
            if i == 0:
                r.append("0")
            break

        if x in nums:
            i += 1
            r.append(x)
            stdscr.addch(r[-1])

    return "".join(r)

curses.wrapper(main)
