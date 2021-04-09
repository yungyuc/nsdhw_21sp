=========================
NSD Term Project Proposal
=========================

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
