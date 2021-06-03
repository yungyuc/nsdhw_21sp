================
NSD Term Project
================

Term project
============

This course requires you to build a software package of a scientific,
mathematic, or numerical application. Everyone needs to choose a subject
individually, and submit the proposal of the term project no later than the end
of the day of the 7th week, in a digital form. You are expected to discuss with
the instructor if you are unsure about how to choose as the subject.

The software package, of which the source code is publicly available, needs to
be hosted on `github.com <https://github.com/>`__. The software needs to be
executable on the `Ubuntu 20.04 LTS <http://releases.ubuntu.com/20.04/>`__ on
64-bit x86 through command line. Building the software package should use a
single command.

Everyone needs to present the project to the class at the end of the course.
The project is graded based on the following points:

* Good engineering infrastructure.
* Correct implementation and solution to the problem.
* Adequate architecture.
* Good presentation.

Any legal subject is OK.

Proposal
========

To turn in your proposal, you need **a PR** and **an E3 entry for the PR URL**.

The proposal is to help you practice writing a specification.  It should
include the following contents:

1. Basic information (including the GitHub repository)
2. Problem to solve
3. Perspective users
4. System architecture
5. API description
6. Engineering infrastructure
7. Schedule

The purpose of a proposal (or a specification) is to enable discussions that
cannot be done with programming language.  For example, source code is not
suitable for describing software architecture.  In `The Architecture of Open
Source Applications <https://aosabook.org/en/index.html>`__, you can see the
many different ways that the developers use to present architecture.  There is
not a fixed way, but the natural language and diagrams are the most common
tools to.  They are probably the most effective ones, too.

I will assume your project to follow the proposal, and help you control the
progress.  But it is not practical to expect a plan to be 100% accurate.  After
the completion you may still modify it.  Please use pull requests to keep the
proposal up-to-date.  I would like to keep track of your progress and help you
maintain the project quality.

You should start writing some code for your project while writing the proposal.
The initial work will help you understand more about what to do.  It is
difficult, if not impossible, to write a proposal without prototyping.  Do not
wait for the proposal completion.

The proposal accounts for 8 points out of the 40 points allocated to the term
project.  Like homework, you will need to create a PR against the ``proposal``
branch (not ``master``).  You need to create a sub-directory using exactly your
GitHub username as your working directory (``nsdhw_21sp/proposal/$username/``).
The hierarchy should be like::

  - nsdhw_21sp (<- repository root)
    - proposal
      - username (<- your working directory)
        - README.rst (<- the main proposal file)
        - ... other files for your proposal

When submitting your proposal, name the PR as ``<username>-proposal**``, e.g.,
``yungyuc-proposal-submission``.  Don't forget to put the PR URL in the E3
entry too.  You can request my review in the PR for discussions.

There is a template (``template.rst``) provided.  It is not a comprehensive
guideline, but may work as a starting point for your proposal file.

Presentation guideline
======================

Each presentation can use at most 25 minutes. It is suggested that the
presentation itself uses about 20 minutes. The 5 minutes are left for questions
and discussions. But presenters may decide how to use their time. Presenters
are expect to prepare computers for presentation. The time for setting up the
computer is included in the allotted presentation time.

If presenters have difficulty in preparing a computer themselves, they may seek
help from the instructor, and resolve the issue one week before their
presentation.

Grading guideline
=================

Software engineering
++++++++++++++++++++

* Build system:
* Testing:
* Version control:
* History quality:
* Issue tracking:
* Documentation:
* Other:

Correctness
+++++++++++

* Existence of golden:
* Quality of golden:
* Order of development:
* Other:

Software architecture
+++++++++++++++++++++

* SOLID:
* API granularity:
* API stability:
* Proper use of high-level language wrapping:
* Level of modularity:

  * C++ code structure:
  * Python code structure:
* Profiling:
* Runtime:
* Memory:
* Design for testing:
* Iterative design:
* Other:

Presentation
++++++++++++

* Technical fluency:
* Slide clarity:
* Time control:
* Appearance:
* Other:

Directory of term projects
==========================

.. _Xi-Plus: https://github.com/Xi-Plus
.. _adkevin3307: https://github.com/adkevin3307
.. _iblis17: https://github.com/iblis17
.. _q40603: https://github.com/q40603
.. _veax-void: https://github.com/veax-void

.. list-table:: Project list
  :header-rows: 1

  * - Author
    - Codename
    - Repository
    - Subject (with link to proposal)
  * - Xi-Plus_
    - sde_
    - `shared-data-encrypter
      <https://github.com/Xi-Plus/shared-data-encrypter>`__
    - `Shared data encrypter <Xi-Plus/README.rst>`__
  * - adkevin3307_
    - bodyiso_
    - `body_isosurface <https://github.com/adkevin3307/body_isosurface>`__
    - `Body IsoSurface <adkevin3307/proposal.md>`__
  * - iblis17_
    - tsjl_
    - `TimeSeries.jl <https://github.com/JuliaStats/TimeSeries.jl>`__
    - `Heterogeneously-typed and Dynamic Time Series Data Structure of
      TimeSeries.jl <iblis17/README.rst>`__
  * - q40603_
    - ptn_
    - `pair_trade_nsd <https://github.com/q40603/pairs_trade_nsd>`__
    - `Pairs Trading <q40603/proposal.md>`__
  * - veax-void_
    - PIGS_
    - `pigs <https://github.com/veax-void/pigs>`__
    - `🐷 PIGS 🐷: Pixelwise Image Graph Segmentation <veax-void/README.md>`__

sde
+++

Author
  Xi-Plus_
Repository
  https://github.com/Xi-Plus/shared-data-encrypter
Subject (with link to proposal)
  `Shared data encrypter <Xi-Plus/README.rst>`__
Summary
  sde is an encryption library to send encrypted data to many people with
  limited number of copies and the recipients don't share the key.

bodyiso
+++++++

Author
  adkevin3307_
Repository
  https://github.com/adkevin3307/body_isosurface
Subject (with link to proposal)
  `Body IsoSurface <adkevin3307/proposal.md>`__
Summary
  Obtain the iso-surface of the intensity field of the scanning for body
  tissue by using the algorithm of `marching cubes
  <https://en.wikipedia.org/wiki/Marching_cubes>`__.

tsjl
++++

.. _TimeSeries.jl: https://github.com/JuliaStats/TimeSeries.jl

Author
  iblis17_
Subject (with link to proposal)
  `Heterogeneously-typed and Dynamic Time Series Data Structure of
  TimeSeries.jl <iblis17/README.rst>`__
Repository
  https://github.com/JuliaStats/TimeSeries.jl
Summary
  In accordance to the Timeseries.jl_ library, make a system of table-like data
  structures to manipulate sorted time axis with variable length time-series
  data of heterogeneous types, and have Timeseries.jl_ to support it.

ptn
+++

Author
  q40603_
Subject (with link to proposal)
  `Pairs Trading <q40603/proposal.md>`__
Repository
  https://github.com/q40603/pairs_trade_nsd
Summary
  Optimize a Pairs Trading engine that aims at Taiwan Stock Market for
  intra-day high-frequency trading.  A goal of the system is to profit in both
  bull and bear market.

PIGS
++++

Author
  veax-void_
Subject (with link to proposal)
  `🐷 PIGS 🐷: Pixelwise Image Graph Segmentation <veax-void/README.md>`__
Repository
  https://github.com/veax-void/pigs
Summary
  Integrate fast C++ code to process image data using graph translation and
  segmentation.

Presentation time table
=======================

.. list-table:: 6/7 7:00-10:00
  :header-rows: 1

  * - ID
    - Time
    - Presenter
    - Project
  * - 1
    - 07:00 - 07:25
    -
    -
  * - 2
    - 07:30 - 07:55
    - q40603_
    - `Pairs Trading <q40603/proposal.md>`__
  * - 3
    - 08:00 - 08:25
    - adkevin3307_
    - `Body IsoSurface <adkevin3307/proposal.md>`__
  * - 4
    - 08:30 - 08:55
    - Xi-Plus_
    - `shared-data-encrypter
      <https://github.com/Xi-Plus/shared-data-encrypter>`__
  * - 5
    - 09:00 - 09:25
    - veax-void_
    - `Pixelwise image graph <https://github.com/veax-void/pigs>`__
  * - 6
    - 09:30 - 09:55
    -
    -

Optional term projects
======================

.. _oscar97123: https://github.com/oscar97123
.. _xyhuang7215: https://github.com/xyhuang7215

.. list-table:: Optiona project list
  :header-rows: 1

  * - Author
    - Codename
    - Repository
    - Subject (with link to proposal)
  * - oscar97123_
    - btb_
    - `Bitcoin-trading-bot
      <https://github.com/oscar97123/Bitcoin-trading-bot>`__
    - `Bitcoin trading bot <oscar97123/proposal.rst>`__
  * - xyhuang7215_
    - SekirOgre_
    - `Sekirogre <https://github.com/xyhuang7215/Sekirogre>`__
    - `SekirOgre: A game engine <xyhuang7215/proposal.rst>`__

btb
+++

Author
  oscar97123_
Subject (with link to proposal)
  `Bitcoin trading bot <oscar97123/proposal.rst>`__
Repository
  https://github.com/JuliaStats/TimeSeries.jl
Summary
  Make a system that is capable of profiting through buying and selling
  bitcoins.  The system needs to analyze the market activities to obtain the
  information for the actions of trading.

SekirOgre
+++++++++

Author
  xyhuang7215_
Subject (with link to proposal)
  `SekirOgre: A game engine <xyhuang7215/proposal.rst>`__
Repository
  https://github.com/xyhuang7215/Sekirogre
Summary
  Make a simple game development system for people who do not have much
  programming experiences to make action adventure games.
