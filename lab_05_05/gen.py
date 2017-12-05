#!/usr/bin/env python3

import sys
import random

def gen(fn, m, n, sparse):
    ret = []
    els = 0
    mtrx = [[0] * n for _ in range(m)]
    pos = [(mi, ni) for ni in range(n) for mi in range(m)]

    while (1 - sparse) > els / (m * n):
        els += 1
        rndx = random.randint(0, len(pos) - 1)
        rpos = pos[rndx]
        del pos[rndx]
        mtrx[rpos[0]][rpos[1]] = random.uniform(0.01, 1000) * (1 - 2 * random.randint(0, 1))

    ret.append("%i %i\n" % (m, n))
    for i in range(m):
        for j in range(n):
            if (abs(mtrx[i][j]) > 1e-2):
                ret.append("%i %i %.2f\n" % (i + 1, j + 1, mtrx[i][j]))
    ret.append("!\n")
    ret = "".join(ret)

    if fn:
        with open(fn, "w") as f:
            f.write(ret)

    return ret

if __name__ == "__main__":
    if len(sys.argv) != 5:
        print("Usage:", sys.argv[0], "out_file m n sparse%")
        exit()

    fn, m, n, sparse = sys.argv[1:5]

    m, n = int(m), int(n)
    sparse = float(sparse) / 100

    print(gen(fn, m, n, sparse))
