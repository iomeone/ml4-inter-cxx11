#pragma once
#include "cek-eval.hpp"

class rt_type : public ctrl_type {
public:
    explicit rt_type (value_type* v);
    void print (std::ostream& out) const;
    void dump (std::ostream& out) const;
    void eval_step (engine_type* vm);
protected:
    cell_type* gcscan (cell_type* scan, int black);
private:
    value_type*  m_value;
};
