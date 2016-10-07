#include "cek-eval.hpp"
#include "cek-closure.hpp"

// function variant selector for value of evaluating result
//
// A function value is a set of the function parameter,
// its body term, and the environment at the lexical scope.
// see function term `cek-fun.cpp`

closure_type::closure_type (symbol x, term_type* m, env_type* e)
    : value_type (), m_id (x), m_exp (m), m_env (e)
{
}

// poor man's pattern matching
bool
closure_type::closure (symbol& x, term_type*& m, env_type*& e)
{
    x = m_id;
    m = m_exp;
    e = m_env;
    return true;
}

// show print style
void
closure_type::print (std::ostream& out) const
{
    printformat (out, "<fun>", m_id, m_exp, m_env);
}

// show dump style
void
closure_type::dump (std::ostream& out) const
{
    printformat (out, "(closure $1x $2p $3p)", m_id, m_exp, m_env);
}

// garbage collection copying
cell_type*
closure_type::gcscan (cell_type* scan, int black)
{
    scan = scan->gccopy (m_exp, black);
    scan = scan->gccopy (m_env, black);
    return scan;
}
