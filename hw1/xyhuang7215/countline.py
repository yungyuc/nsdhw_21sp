#!/usr/bin/env python3

import sys
import os
import os.path

# original code
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

# get variable $PYTHON_BIN
PYTHON_BIN = os.environ.get('PYTHON_BIN')
PYTHON_BIN = str(PYTHON_BIN)

# check if executable exists
# if executable exists, run it. (add "run" to avoid loop)
if os.system('which ' + PYTHON_BIN + ' >/dev/null') != 0:
    sys.exit('exec: ' + PYTHON_BIN + ": not found")
elif sys.argv[-1] != 'run':
    command = PYTHON_BIN + " " + " ".join(sys.argv) + " run"
    os.system(command)
else:
    sys.argv.pop(-1)
    countline() 
