===============================================================================
Proposal for Heterogeneously-typed and Dynamic Time Series Data Structure of TimeSeries.jl
===============================================================================

.. _TimeSeries.jl: https://github.com/JuliaStats/TimeSeries.jl
.. _DataFrames.jl: https://github.com/JuliaData/DataFrames.jl

Basic information
=================

Repository: `TimeSeries.jl`_

`TimeSeries.jl`_
aims to provide a lightweight framework for working with time series data in Julia.

First, we consider three properties needed by a data structure of
time series data[1].

#. Sorted/Unsorted time axis as index
#. Heterogeneous/Homogeneous type for value
#. Dynamic/Static series length, which implacts on memory allocation

The `TimeSeries.jl`_ package provides a sorted time axis, homogeneous-type and static length
time series data structure - ``TimeArray``.
The sorted property guarantees the operation keeps and respects to the order of data.
The static property implies that the length/size of ``TimeArray`` is immutable after
created.
The homogeneous-type implies that the value of the time series is in the same type.
Actually, the underlying implementation of the value container is
a Julia ``Matrix`` or ``Vector`` type.

In case of manipulating the time series data, the data should be sorted against
the time axis. If the data is not sorted, most of operation,
for instance,  ``lag`` and ``lead``, cannot be applied.
So the sorted property should be consider as enabled.


Problem to solve
================

There are demands of table-like data structure that can hold sorted, Heterogeneous type
and dynamic length time series data.




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

#. https://github.com/JuliaStats/TimeSeries.jl/issues/482#issuecomment-777379241
#. https://github.com/JuliaStats/TimeSeries.jl/issues/482
#. https://www.codeproject.com/Articles/168662/Time-Period-Library-for-NET
