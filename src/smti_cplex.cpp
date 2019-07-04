#include <ilcplex/ilocplex.h>
ILOSTLBEGIN

#include "smti.h"

void SMTI::cplex_add_single_constraints(IloEnv *env, IloModel *model,
    const VarMap &vars_lr, const VarMap &vars_rl) {
  // Single stability constraints
  for(auto & one: _ones) {
    for(int two_id: one.prefs()) {
      Agent &two = _twos[two_id - 1];
      IloNumVarArray first_sum(*env);
      for(auto other: one.as_good_as(two)) {
        first_sum.add(*vars_lr.at(one.id()).at(other));
      }
      IloNumVarArray second_sum(*env);
      for(auto other: two.as_good_as(one)) {
        second_sum.add(*vars_lr.at(other).at(two.id()));
      }
      std::stringstream name;
      name << "stab_" << one.id() << "_" << two.id();
      IloConstraint c(1 - IloSum(first_sum) <= IloSum(second_sum));
      c.setName(name.str().c_str());
      model->add(c);
    }
  }
}

void SMTI::cplex_add_merged_constraints(IloEnv *env, IloModel *model,
    const VarMap &vars_lr, const VarMap &vars_rl) {

  // For a two.id(), and an index into two's preference groups,
  // better_than[two.id()][index] is an Ilo array of all variables
  // corresponding to matches "at least as good as" any in the given group of
  // twos preferences.
  std::vector<std::vector<IloNumVarArray>> better_than;

  for(const auto &two: _twos) {
    // Create each empty set of arrays.
    better_than.emplace_back();
    for(int i = 0; i < two.preferences().size(); ++i) {
      better_than[better_than.size() - 1].emplace_back(*env);
    }
  }

  // Fill the better_than array
  for(const auto & one: _ones) {
    for(std::vector<signed int> tie: one.preferences()) {
      for(auto pref: tie) {
        int rank = _twos[pref - 1].rank_of(one.id());
        for(int l = rank; l <= _twos[pref - 1].preferences().size(); ++l) {
          better_than[pref - 1][l - 1].add(*vars_lr.at(one.id()).at(pref));
        }
      }
    }
  }

  for(auto & one: _ones) {
    int group = 0;
    IloNumVarArray left_side(*env);
    for(const std::vector<signed int> & tie: one.preferences()) {
      IloNumVarArray right_side(*env);
      for(signed int pref: tie) {
        left_side.add(*vars_lr.at(one.id()).at(pref));
        int rank = _twos[pref - 1].rank_of(one.id());
        right_side.add(better_than[pref - 1][rank - 1]);
      }
      IloConstraint c(tie.size() * (1 - IloSum(left_side)) <= IloSum(right_side));
      std::stringstream name;
      name << "stab_" << one.id() << "_" << group;
      c.setName(name.str().c_str());
      model->add(c);
      group++;
    }
  }
}


double SMTI::solve_cplex(bool optimise, bool merged) {
  IloEnv env;
  IloModel model(env);
  VarMap vars_lr;
  VarMap vars_rl;
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
  if (!merged) {
    cplex_add_single_constraints(&env, &model, vars_lr, vars_rl);
  } else {
    cplex_add_merged_constraints(&env, &model, vars_lr, vars_rl);
  }
  model.add(IloMaximize(env, IloSum(everything)));
  IloCplex problem(model);
  // Parameters
  // Set number of threads
  problem.setParam(IloCplex::Threads, 1);
  // Hide CPLEX output
  problem.setOut(env.getNullStream());
  problem.exportModel("test.lp");
  double start_time = problem.getCplexTime();
  problem.solve();
  std::cout << (merged ? "merged" : "unmerged") << " took " << (problem.getCplexTime() - start_time) << std::endl;
  return problem.getObjValue();
}
