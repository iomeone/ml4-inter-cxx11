#include "cek-eval.hpp"
#include "cek-fun.hpp"
#include "cek-closure.hpp"
#include "cek-rt-ctrl.hpp"

fun_type::fun_type (symbol x, term_type* m)
    : term_type (), m_id (x), m_exp (m)
{
}

void
fun_type::print (std::ostream& out) const
{
    printformat (out, "(fun $1x -> $2m)", m_id, m_exp);
}

void
fun_type::dump (std::ostream& out) const
{
    printformat (out, "(fun $1x $2p)", m_id, m_exp);
}

// (fun x m) E K -> (Rt (closure x m E)) E K
void
fun_type::eval_step (engine_type* vm)
{
    value_type* v = retain_cell (vm, new closure_type (m_id, m_exp, vm->m_env));
    vm->m_ctrl = retain_cell (vm, new rt_type (v));
}

cell_type*
fun_type::gcscan (cell_type* scan, int black)
{
    scan = scan->gccopy (m_exp, black);
    return scan;
}
