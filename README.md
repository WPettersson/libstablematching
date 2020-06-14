# libstablematching

[![Build Status](https://travis-ci.org/WPettersson/libstablematching.svg?branch=master)](https://travis-ci.org/WPettersson/libstablematching)[![codecov](https://codecov.io/gh/WPettersson/libstablematching/branch/master/graph/badge.svg)](https://codecov.io/gh/WPettersson/libstablematching)

This library provides numerous ways of interacting with various stable matching
problems. For now, we only deal with instances of SMTI, or Stable Matching
problems with Ties and Incomplete lists, where agents do not have any capacity.


For a background on stable matchings, see one of the following:

* Algorithmics of Matching under Preferences. David. Manlove. World Scientific, 2013. [http://www.optimalmatching.com/AMUP](http://www.optimalmatching.com/AMUP)

* Mathematical models for stable matching problems with ties and incomplete lists. M. Delorme, S. García, J. Gondzio, J. Kalcsics, D. Manlove, W.  Pettersson, 2019.  [https://doi.org/10.1016/j.ejor.2019.03.017](https://doi.org/10.1016/j.ejor.2019.03.017)


This software can currently encode MaxSMTI, the problem of finding a largest stable matching, into the following problem types:

 * SAT problem
 * Weighted Pseudo MaxSAT
 * Pseudo Boolean Optimisation
 * Integer Programming Optimisation
 * Pseudo Boolean SAT

## Features

### Preprocessing

libstablematching supports preprocessing as defined in "Mathematical models for
stable matching problems with ties and incomplete lists". This preprocessing
will reduce a stable matching problem by marking as unacceptable pairs that
will never be in any stable matching.
As such, this preprocessing is useful for find optimal stable matchings for a
variety of definitions of optimal.

### Dummy variables

The two SAT encodings check for a complete stable matching. To
check for a largest-size stable matching, one can add dummy agents using
`SMTI::add_dummy().` This adds one dummy agent to either side, each of which
finds all other agents equally acceptable. If adding *n-1* dummy agents results
in an unsatisfiable instance, but adding *n* dummy variables makes the
instances satisfiable, then the largest size stable matching has *n* unmatched
agents.


## Encoding types

### SAT

The produced file is encoded in [DIMACS cnf](http://www.domagoj-babic.com/uploads/ResearchProjects/Spear/dimacs-cnf.pdf).
This encoding only looks for a stable matching, not a maximum-sized one.

### Weighted Pseudo MaxSAT

The produced file is encoded in a similar DIMACS form (see
[http://maxsat.ia.udl.cat/requirements/](http://maxsat.ia.udl.cat/requirements/)).


### Pseudo Boolean Optimisation

This creates files in the [OPB](https://www.cril.univ-artois.fr/PB10/format.pdf) format.
The objective in such an encoding is to minimise the number of agents who are
not matched.


### Minizinc Format

This creates a file suitable for use with [Minizinc](https://www.minizinc.org/), with an optional parameter to toggle between either find any stable matching or find a largest-size stable matching.

### Integer programming optimisation

Currently the IP is directly solved with [Symphony](https://coin-or.github.io/SYMPHONY/).
The basic model is taken from [https://doi.org/10.1016/j.ejor.2019.03.017](https://doi.org/10.1016/j.ejor.2019.03.017) (lines (1) through (5)). Stability constraint merging (as described in [https://doi.org/10.1016/j.ejor.2019.03.017](https://doi.org/10.1016/j.ejor.2019.03.017)) is also supported as an option.
The solver is accessed via OsiSolverInterface, so adaptations to other solvers should not be too difficult.


