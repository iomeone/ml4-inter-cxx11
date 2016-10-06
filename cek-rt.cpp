#include "cek-eval.hpp"
#include "cek-rt-ctrl.hpp"
#include "cek-rt-kont.hpp"
#include "cek-ref.hpp"

rt_type::rt_type (value_type* v)
    : ctrl_type (), m_value (v)
{
}

rt_kont_type::rt_kont_type (symbol x, env_type* e, kont_type* k)
    : kont_type (e, k), m_id (x)
{
}

void
rt_type::print (std::ostream& out) const
{
    printformat (out, "(rt $2m)", 0, m_value);
}

void
rt_kont_type::print (std::ostream& out) const
{
    printformat (out, "(krt $1x #$2p# $3m)", m_id, m_env, m_kont);
}

void
rt_type::dump (std::ostream& out) const
{
    printformat (out, "(rt $2p)", 0, m_value);
}

void
rt_kont_type::dump (std::ostream& out) const
{
    printformat (out, "(krt $1x $2p $3p)", m_id, m_env, m_kont);
}

// (rt v) E K
void
rt_type::eval_step (engine_type* vm)
{
    vm->m_kont->eval_step (vm, m_value);
}

// (rt v) E' (krt x E K) -> (ref x) (env x v E) K
void
rt_kont_type::eval_step (engine_type* vm, value_type* v)
{
    vm->m_ctrl = retain_cell (vm, new ref_type (m_id));
    vm->m_env = retain_cell (vm, new env_type (m_id, v, m_env));
    vm->m_kont = m_kont;
}

cell_type*
rt_type::gcscan (cell_type* scan, int black)
{
    return scan->gccopy (m_value, black);
}

cell_type*
rt_kont_type::gcscan (cell_type* scan, int black)
{
    scan = scan->gccopy (m_env, black);
    scan = scan->gccopy (m_kont, black);
    return scan;
}
