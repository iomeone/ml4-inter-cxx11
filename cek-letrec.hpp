#pragma once
#include "cek-eval.hpp"

class letrec_type : public term_type {
public:
    explicit letrec_type (symbol x, term_type* m1, term_type* m2);
    symbol id (void) const { return m_id; }
    term_type* exp1 (void) const { return m_exp1; }
    term_type* exp2 (void) const { return m_exp2; }
    void print (std::ostream& out) const;
    void dump (std::ostream& out) const;
    void eval_step (engine_type* vm);
protected:
    cell_type* gcscan (cell_type* scan, int black);
private:
    symbol m_id;
    term_type* m_exp1;
    term_type* m_exp2;
};
