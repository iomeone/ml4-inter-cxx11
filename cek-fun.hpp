#pragma once
#include "cek-eval.hpp"

class fun_type : public term_type {
public:
    explicit fun_type (symbol x, term_type* m);
    symbol id (void) const { return m_id; }
    term_type* exp (void) const { return m_exp; }
    void print (std::ostream& out) const;
    void dump (std::ostream& out) const;
    void eval_step (engine_type* vm);
protected:
    cell_type* gcscan (cell_type* scan, int black);
private:
    symbol m_id;
    term_type* m_exp;
};
