======================
Homework assignment #5
======================

To turn in your homework, you need **a PR** and **an E3 entry for the PR URL**.

Question
========

Based on what you wrote in the previous homework assignment or the course
notes, use the provided ``pybind11`` wrapper file ``mod.cpp`` to create the
Python wrapper.  This assignment is to help you practice the modern C++
techniques employed in the provided wrapper file.

The provided wrapper file counts the number that the Python constructor of
``Matrix`` class is called, by using an undocumented feature of ``pybind11``.
The check for the call count is to ensure you are using the provided wrapper
file.  You are **not** asked to understand the mechanism of the checking code.

Note that your code needs to use tiling technique to speed up the matrix-matrix
multiplication without limiting the matrix or tiling size, as you have done in
homework assignment #3.  You may copy this part of code from your previous
submission.

Submission
==========

Your PR needs to be created against the ``hw5`` branch.  (Not ``master``!) You
need to create a sub-directory using exactly your GitHub username as your
working directory (``nsdhw_21sp/hw5/$username/``).  The hierarchy should be::

  - nsdhw_21sp (<- repository root)
    - hw5
      - username (<- your working directory)
        - Makefile
        - xxx.{cc|cpp|cxx}
        - (any other files you need)

In **every commit** in your PR, you can only change the files in your working
directory.  You may not touch anything else.  Failure to follow may cost you
points.

Please make sure your PR is compatible with the latest NSD AMI on AWS (it uses
Ubuntu 20.04 LTS) in ``us-east-1``.  You are not required to use the AMI for
doing the homework, but the grader uses it to run your code.  If your code
fails to build or run on it, **you can lose points**.

There are totally 5 points in this homework assignment.  The grader will run on
a `m5.large` in `us-east-1` and check the result from the ``validate.py``
script.  The benchmark result may go off on different hardware.  Please test
your code in the same or a similar environment.  The grader may attempt to run
the script with the environment variable ``PRELOAD_MKL`` set, if
``validate.py`` doesn't pass without it.  Your work is considered pass in
either case.

CI is set up to provide additional guidance.  The grader may reference the CI
result but will not use it to solely determine your grade.  To get it running,
your branch name should start with ``<username>-hw5**``, e.g,
``yungyuc-hw5-attempt1``.  See the CI configuration file and the Github Action
documentation for more information.

.. vim: set ft=rst ff=unix fenc=utf8 et sw=2 ts=2 sts=2:
