#include "cek-eval.hpp"

// The environment is the list of pair of a variable and a value.

env_type::env_type (symbol x, value_type* v, env_type* e)
    : cell_type (), m_id (x), m_value (v), m_env (e)
{
}

// show print style
void
env_type::print (std::ostream& out) const
{
    printformat (out, "(env $1x $2m $3m)", m_id, m_value, m_env);
}

// show dump style
void
env_type::dump (std::ostream& out) const
{
    printformat (out, "(env $1x $2p $3p)", m_id, m_value, m_env);
}

// lookup an environment just same as symbol with x.
// when not found returns nullptr.
env_type*
env_type::lookup (symbol x)
{
    for (env_type* e = this; e != nullptr; e = e->m_env) {
        if (e->m_id == x && e->m_value != nullptr) {
            return e;
        }
    }
    return nullptr;
}

// garbage collection copying
cell_type*
env_type::gcscan (cell_type* scan, int black)
{
    scan = scan->gccopy (m_value, black);
    scan = scan->gccopy (m_env, black);
    return scan;
}

