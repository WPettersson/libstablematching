#include <ilcplex/ilocplex.h>
ILOSTLBEGIN

#include "smti.h"


double SMTI::solve_cplex(bool optimise) {
  IloEnv env;
  IloModel model(env);
  std::map<int, std::map<int, IloBoolVar*>> vars_lr;
  std::map<int, std::map<int, IloBoolVar*>> vars_rl;
  IloNumVarArray everything(env);
  for(auto & one: _ones) {
    for(int two_id: one.prefs()) {
      Agent & two = _twos[two_id-1];
      if (vars_lr.count(one.id()) == 0) {
        vars_lr.emplace(one.id(), std::map<int, IloBoolVar*>());
      }
      if (vars_rl.count(two_id) == 0) {
        vars_rl.emplace(two_id, std::map<int, IloBoolVar*>());
      }
      std::stringstream name;
      name << "x" << one.id() << "_" << two.id();
      IloBoolVar *var = new IloBoolVar(env, name.str().c_str());
      everything.add(*var);
      vars_lr[one.id()][two_id] = var;
      vars_rl[two_id][one.id()] = var;
    }
  }
  // Ones capacity
  for(auto & one: _ones) {
    if (one.prefs().size() == 0) {
      continue;
    }
    IloNumVarArray capacity(env);
    for(int two_id: one.prefs()) {
      capacity.add(*vars_lr[one.id()][two_id]);
    }
    std::stringstream name;
    name << "cap_one_" << one.id();
    if (optimise) {
      IloConstraint c(IloSum(capacity) <= 1);
      c.setName(name.str().c_str());
      model.add(c);
    } else {
      IloConstraint c(IloSum(capacity) == 1);
      c.setName(name.str().c_str());
      model.add(c);
    }
  }
  // Twos capacity
  for(auto & two: _twos) {
    if (two.prefs().size() == 0) {
      continue;
    }
    IloNumVarArray capacity(env);
    for(int one_id: two.prefs()) {
      capacity.add(*vars_lr[one_id][two.id()]);
    }
    std::stringstream name;
    name << "cap_two_" << two.id();
    IloConstraint c(IloSum(capacity) <= 1);
    c.setName(name.str().c_str());
    model.add(c);
  }
  // Stability constraints
  for(auto & one: _ones) {
    for(int two_id: one.prefs()) {
      Agent &two = _twos[two_id - 1];
      IloNumVarArray first_sum(env);
      for(auto other: one.as_good_as(two)) {
        first_sum.add(*vars_lr[one.id()][other]);
      }
      IloNumVarArray second_sum(env);
      for(auto other: two.as_good_as(one)) {
        second_sum.add(*vars_lr[other][two.id()]);
      }
      std::stringstream name;
      name << "stab_" << one.id() << "_" << two.id();
      IloConstraint c(1 - IloSum(first_sum) <= IloSum(second_sum));
      c.setName(name.str().c_str());
      model.add(c);
    }
  }
  model.add(IloMaximize(env, IloSum(everything)));
  IloCplex problem(model);
  // Parameters
  // Set number of threads
  problem.setParam(IloCplex::Threads, 1);
  // Hide CPLEX output
  problem.setOut(env.getNullStream());
  problem.exportModel("test.lp");
  problem.solve();
  return problem.getObjValue();
}
