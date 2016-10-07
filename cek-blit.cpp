#include "cek-eval.hpp"
#include "cek-blit.hpp"
#include "cek-value-bool.hpp"
#include "cek-rt-ctrl.hpp"

// boolean literal term
//
//     function (exp, env, kont) {
//       kont (new value_bool_type (exp.lit));
//     }

blit_type::blit_type (bool b)
    : term_type (), m_lit (b)
{
}

// show print style: "true" / "false"
void
blit_type::print (std::ostream& out) const
{
    printformat (out, "$1b", m_lit);
}

// show dump style
void
blit_type::dump (std::ostream& out) const
{
    printformat (out, "(blit $1b)", m_lit);
}

// evaluation

// (blit c) E K -> (rt (boolv c)) E K
void
blit_type::eval_step (engine_type* vm)
{
    value_type* v = retain_cell (vm, new value_bool_type (m_lit));
    vm->m_ctrl = retain_cell (vm, new rt_type (v));
}

// use default garbage collection copying due to no child cell
