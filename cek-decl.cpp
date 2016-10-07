#include "cek-eval.hpp"
#include "cek-decl.hpp"
#include "cek-decl-kont.hpp"
#include "cek-rt-kont.hpp"

// declaration `let x = exp;;` extends the environment.

decl_type::decl_type (symbol x, term_type* m)
    : term_type (), m_id (x), m_exp (m)
{
}

decl_kont_type::decl_kont_type (symbol x, env_type* e, kont_type* k)
    : kont_type (e, k), m_id (x)
{
}

// show print style
void
decl_type::print (std::ostream& out) const
{
    printformat (out, "(let $1x = $2m)", m_id, m_exp);
}

void
decl_kont_type::print (std::ostream& out) const
{
    printformat (out, "(kdecl $1x #$2p# $3m)", m_id, m_env, m_kont);
}

// show dump style
void
decl_type::dump (std::ostream& out) const
{
    printformat (out, "(decl $1x $2m)", m_id, m_exp);
}

void
decl_kont_type::dump (std::ostream& out) const
{
    printformat (out, "(kdecl $1x $2p $3p)", m_id, m_env, m_kont);
}

// evaluation

// (decl x m) E K -> m E (kdecl x E K)
void
decl_type::eval_step (engine_type* vm)
{
    vm->m_kont = retain_cell (vm,
        new decl_kont_type (m_id, vm->m_env, vm->m_kont));
    vm->m_ctrl = m_exp;
}

// replace rt_kont_type to extend the environment.

// (rt v) E' (kdecl x E0 (krt x' E0 K)) -> (rt v) E1=(env x v E0) (krt x E1 K)
void
decl_kont_type::eval_step (engine_type* vm, value_type* v)
{
    vm->m_env = retain_cell (vm, new env_type (m_id, v, m_env));
    vm->m_kont = retain_cell (vm,
        new rt_kont_type (m_id, vm->m_env, vm->m_kont->kont ()-> kont ()));
}

// garbage collection copying
cell_type*
decl_type::gcscan (cell_type* scan, int black)
{
    scan = scan->gccopy (m_exp, black);
    return scan;
}

cell_type*
decl_kont_type::gcscan (cell_type* scan, int black)
{
    scan = scan->gccopy (m_env, black);
    scan = scan->gccopy (m_kont, black);
    return scan;
}
