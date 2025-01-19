/**
* @author Tim Luchterhand
* @date 26.11.24
* @brief
*/

#include "basic_structures.hpp"
#include "util/exception.hpp"

namespace sat {
    // @TODO implementation here


    Variable::Variable(unsigned val): val(val) {

    }

    unsigned Variable::get() const {
        return val;
    }

    bool Variable::operator==(Variable other) const {
        return other.get()== val ;
    }

    Literal::Literal(unsigned val): val(val) {
        
    }

    unsigned Literal::get() const {
        return val;
    }

    Literal Literal::negate() const {
        return val^1 ;
    }

    short Literal::sign() const {
        if (val % 2){
            return 1;
        }
        else{
            return -1;
        }
    }

    bool Literal::operator==(Literal other) const {
        return other.get()==val ;
    }

    Literal pos(Variable x) {
        return Literal(2*x.get()+1);
    }

    Literal neg(Variable x) {
        return Literal(2*x.get());
    }

    Variable var(Literal l) {
        return Variable(l.get() / 2);
    }
}
