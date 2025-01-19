/**
* @author Tim Luchterhand
* @date 27.11.24
* @brief
*/

#include "Solver.hpp"
#include "util/exception.hpp"

namespace sat {
    Solver::Solver(unsigned numVariables) {
        this->numVariables=numVariables;
        this->clauses = std::vector<ClausePointer>();
        this->model = std::vector<TruthValue>(numVariables, TruthValue::Undefined); 
        this->unitLiterals = std::vector<Literal>(); 
        this->watchedBg = std::vector<std::vector<ClausePointer>>();
    }

    bool Solver::addClause(Clause clause) {
        if (clause.isEmpty()) {
            return false;
        }

        if (clause.size() == 1) {
            if (falsified(clause[0])) {
                return false;
            }
            assign(clause[0]);
        }

        clauses.push_back(std::make_shared<Clause>(std::move(clause)));
        return true;
    }

    auto Solver::rebase() const -> std::vector<Clause> {
        std::vector<Clause> res;

        for (ClausePointer clausePtr : clauses) {
            bool satisfiedClause = false;
            std::vector<Literal> remainingLiterals;

            for (const Literal& l : *clausePtr) {
                if (satisfied(l)) {
                    satisfiedClause = true;
                    res.emplace_back(std::vector<Literal>{l});
                    break;
                } else if (!falsified(l)) {
                    remainingLiterals.push_back(l);
                }
            }

            if (!satisfiedClause) {
                res.emplace_back(std::move(remainingLiterals));
            }
        }

        return res;
    }

    TruthValue Solver::val(Variable x) const {
        return (this->model.at(x.get()));
    }

    bool Solver::satisfied(Literal l) const {
        TruthValue truth = val(var(l));
        return (truth == TruthValue::True && l.sign() == 1) || (truth == TruthValue::False && l.sign() == -1);
        
    }

    bool Solver::falsified(Literal l) const {
        TruthValue truth = val(var(l));
        return truth != TruthValue::Undefined && !satisfied(l);
    }

    bool Solver::assign(Literal l) {
        bool val = !falsified(l);
        if (val && !satisfied(l)){
            this->model.at(var(l).get())= l.sign()==1 ? TruthValue::True : TruthValue::False;
            this->unitLiterals.push_back(l);
        }
        return val; 
    }

    bool Solver::unitPropagate() {
        bool result = true;

        while (true) {
            bool hasNewUnit = false;

            for (const Clause& clause : rebase()) {
                if (!addClause(clause)) {
                    result = false;
                }
                if (clause.size() == 1) {
                    hasNewUnit = true;
                }
            }

            if (!hasNewUnit) {
                break;
            }
        }

        return result;
    }
} // sat