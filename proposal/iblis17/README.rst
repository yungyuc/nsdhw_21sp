===============================================================================
Proposal for Heterogeneously-typed and Dynamic Time Series Data Structure of TimeSeries.jl
===============================================================================

.. _TimeSeries.jl: https://github.com/JuliaStats/TimeSeries.jl

Basic information
=================

Repository: `TimeSeries.jl`_

`TimeSeries.jl`_
aims to provide a lightweight framework for working with time series data in Julia.

This package provide a homogeneous-type static time series data structure -
``TimeArray``.
The static property implies that the length/size of ``TimeArray`` is immutable after
created.
The homogeneous-type implies that the value of the time series is in the same type.
Actually, the underlying implementation of the value container is
a Julia ``Matrix`` or ``Vector`` type.


Problem to solve
================

In case of manipulating the time series data, the data should be sorted against
the time axis. If the data is not sorted, most of operation,
for instance,  ``lag`` and ``lead``, cannot be applied.


Perspective users
=================


System architecture
===================

API description
===============

Engineering infrastructure
==========================

1. Testing framework
2. CI system
3. Release Procedure

Schedule
========

References
==========

#. https://github.com/JuliaStats/TimeSeries.jl/issues/482
#. https://www.codeproject.com/Articles/168662/Time-Period-Library-for-NET
