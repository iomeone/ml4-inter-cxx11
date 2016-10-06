#pragma once
#include "cek-eval.hpp"

class stop_kont_type : public kont_type {
public:
    stop_kont_type ();
    bool isstop (void) const { return true; }
    void print (std::ostream& out) const;
    void dump (std::ostream& out) const;
    void eval_step (engine_type* vm, value_type* v);
};
