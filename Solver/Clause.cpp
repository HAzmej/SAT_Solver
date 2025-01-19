/**
* @author Tim Luchterhand
* @date 26.11.24
* @brief
*/

#include <cassert>
#include <algorithm>

#include "Clause.hpp"
#include "util/exception.hpp"

namespace sat {
    //TODO implementation here

    Clause::Clause(std::vector<Literal> literals) {
        lit=literals;
        std::ranges
    }

    short Clause::getRank(Literal l) const {
        if( lit.at(Watcher)==1){
            return 0;
        }
        if (lit.at(Watcher2)==1){
            return 1;
        }
        return -1;
    }

    std::size_t Clause::getIndex(short rank) const {
        return (rank ==0) ? Watcher1 : Watcher2 ;
    }

    bool Clause::setWatcher(Literal l, short watcherNo) {
        auto it = std::find(literals.begin(), literals.end(), l);
        if (it == literals.end()) {
            return false;
        }

        std::size_t index = std::distance(literals.begin(), it);
        if (watcherNo == 0) {
            Watcher1 = index;
        } else {
            Watcher2 = index;
        }
        return true;
    }

    auto Clause::begin() const -> std::vector<Literal>::const_iterator {
        return lit.cbegin();
    }

    auto Clause::end() const -> std::vector<Literal>::const_iterator {
        return lit.cend();
    }

    bool Clause::isEmpty() const {
        return lit.empty();
    }

    Literal Clause::operator[](std::size_t index) const {
        //Verif si l'index est valide
        if index < lit.size(){
            return lit[index];
        }
        return -1
    }

    std::size_t Clause::size() const {
        return lit.size();
    }

    Literal Clause::getWatcherByRank(short rank) const {
        if (rank==0) {return lit[Watcher1];}
        if (rank==1) {return lit[Watcher2];}
        return -1;
    }

    bool Clause::sameLiterals(const Clause &other) const {
        //par Taille
        if (lit.size() != other.size()){
            return false;
        }
        else {
            for (int i =0;i<other.size();i++){
                if (lit[i]!=other[i]) {return false;}
            }
        }
        return true
    }

}
