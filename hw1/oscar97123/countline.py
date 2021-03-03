#!/usr/bin/env python3

import os
import sys
import os.path

env_var = os.getenv('PYTHON_BIN')

def countline():
    if len(sys.argv) < 2:
        sys.stdout.write('missing file name\n')
    elif len(sys.argv) > 2:
        sys.stdout.write('only one argument is allowed\n')
    else:
        fname = sys.argv[1]
        if os.path.exists(fname):
            with open(fname) as fobj:
                lines = fobj.readlines()
            sys.stdout.write('{} lines in {}\n'.format(len(lines), fname))
        else:
            sys.stdout.write('{} not found\n'.format(fname))

if env_var != "python2" and env_var != "python3":
    sys.exit("exec: " + env_var + ": not found")
else:
    countline()

