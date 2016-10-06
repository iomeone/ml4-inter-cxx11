#pragma once
#include "cek-eval.hpp"

class blit_type : public term_type {
public:
    explicit blit_type (bool b);
    void print (std::ostream& out) const;
    void dump (std::ostream& out) const;
    void eval_step (engine_type* vm);
private:
    bool m_lit;
};
