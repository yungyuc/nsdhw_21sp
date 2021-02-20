======================
Homework assignment #1
======================

To turn in your homework, you need **a PR** and **an E3 entry for the PR URL**.

Your PR needs to be created against the ``hw1`` branch.  (Not ``master``!) You
need to create a sub-directory using exactly your GitHub username as your
working directory (``nsdhw_21sp/hw1/$username/``).  The hierarchy should be::

  - nsdhw_21sp (<- repository root)
    - hw1
      - username (<- your working directory)
        - Your files

In **every commit** in your PR, you can only change the files in your working
directory.  You may not touch anything else.  Failure to follow may cost you
points.

Please make sure your PR passes the Github Action CI, and is compatible with
the latest NSD AMI on AWS (it uses Ubuntu 20.04 LTS) in ``us-east-1``.  You are
not required to use the AMI for doing the homework, but the grader is.  If your
code fails to build or run on it, **you can lose all points**.

Everyone should write his/her own code.  It is OK to discuss, but there should
not be duplicated code.  If duplication is found, **all points** for the
duplicated part of the latter submitter may be deducted.

Question
========

1. Use bash to reimplement ``countline.py``.  You may not call Python.
2. Modify the script ``countline.py`` so that it reads the environment variable
   named "``PYTHON_BIN``" to switch the Python executable for the script.  The
   script must be runnable by both ``python2`` and ``python3``.  It should
   exit with an error code when the specified binary is not found.

   Hint: play a trick using bash.

Grading guideline
=================

This homework assignment has 5 points.  On the latest NSD AMI, the grader will
run the following commands:

.. code-block:: bash

  cd nsdhw_21sp/hw1/username
  ../validate.py | grep "GET POINT"

.. vim: set ft=rst ff=unix fenc=utf8 et sw=2 ts=2 sts=2:
