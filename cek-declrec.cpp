#include "cek-eval.hpp"
#include "cek-declrec.hpp"
#include "cek-declrec-kont.hpp"
#include "cek-rt-kont.hpp"

// declare recursive function `let rec x = exp;;`
// where exp must be a function `fun x -> exp`.

declrec_type::declrec_type (symbol x, term_type* m)
    : term_type (), m_id (x), m_exp (m)
{
}

declrec_kont_type::declrec_kont_type (symbol x, env_type* e, kont_type* k)
    : kont_type (e, k), m_id (x)
{
}

// show print style
void
declrec_type::print (std::ostream& out) const
{
    printformat (out, "(let rec $1x = $2m)", m_id, m_exp);
}

void
declrec_kont_type::print (std::ostream& out) const
{
    printformat (out, "(kdeclrec $1x #$2p# $3m)", m_id, m_env, m_kont);
}

// show dump style
void
declrec_type::dump (std::ostream& out) const
{
    printformat (out, "(declrec $1x $2m)", m_id, m_exp);
}

void
declrec_kont_type::dump (std::ostream& out) const
{
    printformat (out, "(kdeclrec $1x $2p $3p)", m_id, m_env, m_kont);
}

// evaluate a function under the dummy environment.
// The dummy environment binds nullptr to variable x.

// (declrec x m) E K -> m E1=(x DUMMY E) (kdeclrec x E1 K)
void
declrec_type::eval_step (engine_type* vm)
{
    vm->m_env = retain_cell (vm, new env_type (m_id, nullptr, vm->m_env));
    vm->m_kont = retain_cell (vm,
        new declrec_kont_type (m_id, vm->m_env, vm->m_kont));
    vm->m_ctrl = m_exp;
}

// replace the dummy environment with the evaluated function.

// (rt v) E' (kdeclrec x E1=(x DUMMY E) (krt E0 K)) -> (rt v) E1=(env x v E) (krt x E1 K)
void
declrec_kont_type::eval_step (engine_type* vm, value_type* v)
{
    vm->m_env = m_env;
    vm->m_env->set_value (v);
    vm->m_kont = retain_cell (vm,
        new rt_kont_type (m_id, vm->m_env, vm->m_kont->kont ()->kont ()));
}

// garbage collection copying
cell_type*
declrec_type::gcscan (cell_type* scan, int black)
{
    scan = scan->gccopy (m_exp, black);
    return scan;
}

cell_type*
declrec_kont_type::gcscan (cell_type* scan, int black)
{
    scan = scan->gccopy (m_env, black);
    scan = scan->gccopy (m_kont, black);
    return scan;
}
