===============================================================================
Proposal for Heterogeneously-typed and Dynamic Time Series Data Structure of TimeSeries.jl
===============================================================================

.. _TimeSeries.jl: https://github.com/JuliaStats/TimeSeries.jl
.. _DataFrames.jl: https://github.com/JuliaData/DataFrames.jl

Basic information
===============================================================================

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
===============================================================================

There are demands of table-like data structures that can hold sorted time axis,
heterogeneous type and dynamic length time series data.
This proposal proposes to build a series of data type, interface and operations
that can fulfill the demands.


Perspective users
===============================================================================



System architecture
===============================================================================


API descriptions
===============================================================================

Time Type
----------------------------------------------------------------------

The supported time types can be used as elements of time axis.

#. ``Dates.TimeType``, the abstract time type from Julia's stdlib.
#. ``Dates.Period``, the abstract period type from Julia's stdlib.
   It is used for a time interval or a time offset relative to some reference
   point.


Vector of Timestamps
----------------------------------------------------------------------

A vector of timestamps should be sorted, in order to apply time-related operations.
Allowing repeating timestamp or not is one of the key properties.
In this proposal, repeating timestamp in a vector should be allowed.
In some real cases, sensor generated timestamps might be the same due to the
inaccurate timer or time resolution,
but allow repeating does introduce some API design issues.

In case of repeated timestamps, it can be solved by introduce the secondary index,
assume that the order from user input is in ascending order.
The secondary index can be automatically built. For instance:

+----------+-----------------+--------+
| Timestap | Secondary Index | Valueu |
+==========+=================+========+
| 10:00:00 | 1               | foo    |
+----------+-----------------+--------+
| 10:00:00 | 2               | bar    |
+----------+-----------------+--------+
| 10:00:00 | 3               | baz    |
+----------+-----------------+--------+
| 10:30:00 | 1               | qaz    |
+----------+-----------------+--------+
| ...      |                 |        |
+----------+-----------------+--------+

The we can provides two sets of APIs.
#. All the APIs in the first set will take ``secondary index = 1`` and compute.
   This set is designed for the case that there aren't repeating in the timestamps,
   or user want to ignore the repeating in specific operations.
#. The second set can accept the secondary index as a argument, and the
   return type will be different from the first set of APIs. Most of APIs in this
   set should return the table type that same or similar to the input table.



Engineering infrastructure
==========================

Testing Framework
----------------------------------------------------------------------

The project `TimeSeries.jl`_ already adopts the testing framework of Julia stdlib --
``Test`` library.
In this proposal, all the changes should be shipped with test cases that can
cover as more as possible.
The code is located at the ``tests/`` dir.

The coverage service is `Coveralls <https://coveralls.io/>`_, which has been
enabled for the project already.


CI System
----------------------------------------------------------------------

The project `TimeSeries.jl`_ use github action as the CI system infrastructure.
It conducts three jobs.

#. Run the test cases by driving the testing framework
   on any push event and PR opened.
   The test matrix is the Julia LTS version and the latest stable version, which are
   v1.0 and v1.6 at the moment of this proposal written.
#. Deploy the documentation to the Github Page,
   triggered by the ``master`` branch push, tagging new release and the pull request push
   event.
#. Creat new tag and update the release changelog automatically.
   This is done by a Github action named *TagBot* in Julia's community.


Release Policy
----------------------------------------------------------------------

Since this project is still in beta stage of development and don't
reach the v1.0, the versioning policy is that the changes of minor version number
implies some breaking changes.


Schedule
===============================================================================



References
===============================================================================

#. https://github.com/JuliaStats/TimeSeries.jl/issues/482#issuecomment-777379241
#. https://github.com/JuliaStats/TimeSeries.jl/issues/482
#. https://www.codeproject.com/Articles/168662/Time-Period-Library-for-NET
