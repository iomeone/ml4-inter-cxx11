#pragma once
#include "cek-eval.hpp"

class prim2_kont_type : public kont_type {
public:
    explicit prim2_kont_type (int op, value_type* v, env_type* e, kont_type* k);
    int op (void) const { return m_op; }
    value_type* value (void) const { return m_value; }
    void print (std::ostream& out) const;
    void dump (std::ostream& out) const;
    void eval_step (engine_type* vm, value_type* v);
protected:
    cell_type* gcscan (cell_type* scan, int black);
private:
    int m_op;
    value_type* m_value;
};
