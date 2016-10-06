#pragma once
#include "cek-eval.hpp"

class if_type : public term_type {
public:
    explicit if_type (term_type* m1, term_type* m2, term_type* m3);
    term_type* exp1 (void) const { return m_exp1; }
    term_type* exp2 (void) const { return m_exp2; }
    term_type* exp3 (void) const { return m_exp3; }
    void print (std::ostream& out) const;
    void dump (std::ostream& out) const;
    void eval_step (engine_type* vm);
protected:
    cell_type* gcscan (cell_type* scan, int black);
private:
    term_type* m_exp1;
    term_type* m_exp2;
    term_type* m_exp3;
};
