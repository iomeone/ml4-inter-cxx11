#pragma once
#include "cek-eval.hpp"

class ref_type : public term_type {
public:
    explicit ref_type (symbol x);
    symbol id (void) const { return m_id; }
    void print (std::ostream& out) const;
    void dump (std::ostream& out) const;
    void eval_step (engine_type* vm);
private:
    symbol m_id;
};
