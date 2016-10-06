#pragma once
#include "cek-eval.hpp"

class value_bool_type : public value_type {
public:
    explicit value_bool_type (bool b);
    void print (std::ostream& out) const;
    void dump (std::ostream& out) const;
    bool boolv (bool& b) const;
private:
    bool m_value;
};
