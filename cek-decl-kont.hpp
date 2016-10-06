#pragma once
#include "cek-eval.hpp"

class decl_kont_type : public kont_type {
public:
    explicit decl_kont_type (symbol x, env_type* e, kont_type* k);
    symbol id (void) const { return m_id; }
    void print (std::ostream& out) const; 
    void dump (std::ostream& out) const;
    void eval_step (engine_type* vm, value_type* v);
protected:
    cell_type* gcscan (cell_type* scan, int black);
private:
    symbol m_id;
};
