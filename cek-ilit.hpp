#pragma once
#include "cek-eval.hpp"

class ilit_type : public term_type {
public:
    explicit ilit_type (int i);
    void print (std::ostream& out) const;
    void dump (std::ostream& out) const;
    void eval_step (engine_type* vm);
private:
    int m_lit;
};
