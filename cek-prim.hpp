#pragma once
#include "cek-eval.hpp"

class prim_type : public term_type {
public:
    explicit prim_type (int op, term_type* m1, term_type* m2);
    int op (void) const { return m_op; }
    term_type* exp1 (void) const { return m_exp1; }
    term_type* exp2 (void) const { return m_exp2; }
    void print (std::ostream& out) const;
    void dump (std::ostream& out) const;
    void eval_step (engine_type* vm);
protected:
    cell_type* gcscan (cell_type* scan, int black);
private:
    int m_op;
    term_type* m_exp1;
    term_type* m_exp2;
};
