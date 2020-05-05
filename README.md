# SMTI encoder

[![Build Status](https://travis-ci.org/WPettersson/smti-encoder.svg?branch=master)](https://travis-ci.org/WPettersson/smti-encoder)[![codecov](https://codecov.io/gh/WPettersson/smti-encoder/branch/master/graph/badge.svg)](https://codecov.io/gh/WPettersson/smti-encoder)

This software creates random SMTI instances, and produces various types of
encodings for said instance for use in benchmarking various types of solvers.
For a background on what SMTI is, see the introduction in

Mathematical models for stable matching problems with ties and incomplete
lists. M. Delorme, S. García, J. Gondzio, J. Kalcsics, D. Manlove, W.
Pettersson.
[https://doi.org/10.1016/j.ejor.2019.03.017](https://doi.org/10.1016/j.ejor.2019.03.017)

This software currently supports the following problem types:

 * SAT
 * Weighted Pseudo MaxSAT
 * Pseudo Boolean Optimisation
 * Integer Programming Optimisation

Next steps might include:

 * Pseudo Boolean SAT

## Features

### Dummy variables

The two SAT encodings allow one to check for a complete stable matching. To
check for a largest-size stable matching, one can add dummy agents using
SMTI::add_dummy(). This adds one dummy agent to either side, each of which
finds all other agents equally acceptable. If adding *n-1* dummy agents results
in an unsatisfiable instance, but adding *n* dummy variables makes the
instances satisfiable, then the largest size stable matching has *n* unmatched
agents.



## Encoding types

### SAT

The SAT encoding produced is in DIMACS cnf form (see
[https://github.com/elffersj/roundingsat/blob/master/InputFormats.md](https://github.com/elffersj/roundingsat/blob/master/InputFormats.md)).

### Weighted Pseudo MaxSAT

The encoding is in a similar DIMACS form (see
[http://maxsat.ia.udl.cat/requirements/](http://maxsat.ia.udl.cat/requirements/)).

### Pseudo Boolean Optimisation

This uses the OPB format as described in
[https://github.com/elffersj/roundingsat/blob/master/InputFormats.md](https://github.com/elffersj/roundingsat/blob/master/InputFormats.md).
The objective in such an encoding is to minimise the number of agents who are
not matched.

### Integer programming optimisation

This actually calls CPLEX directly, but CPLEX creates an LP file as a
by-product.
The objective here is to maximise the number of matched pairs. This means that
the total number of matched agents is twice the objective.


