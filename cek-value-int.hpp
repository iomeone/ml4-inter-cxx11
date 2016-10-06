#pragma once
#include "cek-eval.hpp"

class value_int_type : public value_type {
public:
    explicit value_int_type (int i);
    void print (std::ostream& out) const;
    void dump (std::ostream& out) const;
    bool intv (int& i) const;
private:
    int m_value;
};
