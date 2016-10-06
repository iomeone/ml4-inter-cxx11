#include "cek-eval.hpp"

env_type::env_type (symbol x, value_type* v, env_type* e)
    : cell_type (), m_id (x), m_value (v), m_env (e)
{
}

void
env_type::print (std::ostream& out) const
{
    printformat (out, "(env $1x $2m $3m)", m_id, m_value, m_env);
}

void
env_type::dump (std::ostream& out) const
{
    printformat (out, "(env $1x $2p $3p)", m_id, m_value, m_env);
}

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

cell_type*
env_type::gcscan (cell_type* scan, int black)
{
    scan = scan->gccopy (m_value, black);
    scan = scan->gccopy (m_env, black);
    return scan;
}

