#pragma once
#include "cek-eval.hpp"

class let_kont_type : public kont_type {
public:
    explicit let_kont_type (symbol x, term_type* m2, env_type* e, kont_type* k);
    symbol id (void) const { return m_id; }
    term_type* exp2 (void) const { return m_exp2; }
    void print (std::ostream& out) const;
    void dump (std::ostream& out) const;
    void eval_step (engine_type* vm, value_type* v);
protected:
    cell_type* gcscan (cell_type* scan, int black);
private:
    symbol m_id;
    term_type* m_exp2;
};
