#include "cek-eval.hpp"
#include "cek-ilit.hpp"
#include "cek-value-int.hpp"
#include "cek-rt-ctrl.hpp"

// integer literal term
//
//     function (exp, env, kont) {
//       kont (new value_int_type (exp.lit));
//     }

ilit_type::ilit_type (int i)
    : term_type (), m_lit (i)
{
}

// show print style
void
ilit_type::print (std::ostream& out) const
{
    printformat (out, "$1d", m_lit);
}

// show dump style
void
ilit_type::dump (std::ostream& out) const
{
    printformat (out, "(ilit $1d)", m_lit);
}

// evaluation

// (ilit c) E K -> (rt (intv c)) E K
void
ilit_type::eval_step (engine_type* vm)
{
    value_type* v = retain_cell (vm, new value_int_type (m_lit));
    vm->m_ctrl = retain_cell (vm, new rt_type (v));
}

// use default garbage collection copying due to no child cell
