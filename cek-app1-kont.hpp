#pragma once
#include "cek-eval.hpp"

class app1_kont_type : public kont_type {
public:
    explicit app1_kont_type (term_type* m2, env_type* e, kont_type* k);
    term_type* exp2 (void) const { return m_exp2; }
    void print (std::ostream& out) const;
    void dump (std::ostream& out) const;
    void eval_step (engine_type* vm, value_type* v);
protected:
    cell_type* gcscan (cell_type* scan, int black);
private:
    term_type* m_exp2;
};
