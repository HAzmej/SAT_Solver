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
        if (clause.isEmpty()){
            return false;
        } else if (clause.size()==1 && falsified(clause[0])){
            return false;
        } else if (clause.size()==1 ){
            assign(clause[0]);
        }
        ClausePointer clause_pntr =  std::make_shared<Clause>(clause);
        clauses.push_back(clause_pntr);
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
        bool resultat = true;

        while (!unitLiterals.empty()){
            Literal unit = unitLiterals.back();
            unitLiterals.pop_back();
            if(!assign(unit)){return false;}
            std::vector<Clause> new_clauses = rebase();
            for (const auto& clause :new_clauses){
                resultat = resultat && addClause(clause);
                if (clause.size() == 1) {
                    Literal lit = clause[0];
                    if (val(var(lit)) == TruthValue::Undefined) {
                        unitLiterals.push_back(lit);
                        }
                    }
            }

            if(unitLiterals.empty()){break;}

        }

        return resultat;
    }
} // sat