#pragma once
#include "cek-eval.hpp"

class closure_type : public value_type {
public:
    explicit closure_type (symbol x, term_type* m, env_type* e);
    void print (std::ostream& out) const;
    void dump (std::ostream& out) const;
    bool closure (symbol& x, term_type*& m, env_type*& e);
protected:
    cell_type* gcscan (cell_type* scan, int black);
private:
    symbol m_id;
    term_type* m_exp;
    env_type* m_env;
};
