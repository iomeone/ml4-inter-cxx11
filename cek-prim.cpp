#include "cek-eval.hpp"
#include "cek-prim.hpp"
#include "cek-prim1-kont.hpp"
#include "cek-prim2-kont.hpp"
#include "cek-value-bool.hpp"
#include "cek-value-int.hpp"
#include "cek-rt-ctrl.hpp"
#include "cek-error.hpp"

prim_type::prim_type (int op, term_type* m1, term_type* m2)
    : term_type (), m_op (op), m_exp1 (m1), m_exp2 (m2)
{
}

prim1_kont_type::prim1_kont_type (int op, term_type* m2, env_type* e, kont_type* k)
    : kont_type (e, k), m_op (op), m_exp2 (m2)
{
}

prim2_kont_type::prim2_kont_type (int op, value_type* v, env_type* e, kont_type* k)
    : kont_type (e, k), m_op (op), m_value (v)
{
}

void
prim_type::print (std::ostream& out) const
{
    printformat (out, "($2m $1o $3m)", m_op, m_exp1, m_exp2);
}

void
prim1_kont_type::print (std::ostream& out) const
{
    printformat (out, "(kprim1 $1o $2m #$3p# $4m)", m_op, m_exp2, m_env, m_kont);
}

void
prim2_kont_type::print (std::ostream& out) const
{
    printformat (out, "(kprim2 $1o $2m #$3p# $4m)", m_op, m_value, m_env, m_kont);
}

void
prim_type::dump (std::ostream& out) const
{
    printformat (out, "(prim $1o $2p $3p)", m_op, m_exp1, m_exp2);
}

void
prim1_kont_type::dump (std::ostream& out) const
{
    printformat (out, "(kprim1 $1o $2p $3p $4p)", m_op, m_exp2, m_env, m_kont);
}

void
prim2_kont_type::dump (std::ostream& out) const
{
    printformat (out, "(kprim2 $1o $2p $3p $4p)", m_op, m_value, m_env, m_kont);
}

// (prim op m1 m2) E K -> m1 E (kprim1 op m2 E K)
void
prim_type::eval_step (engine_type* vm)
{
    vm->m_kont = retain_cell (vm,
        new prim1_kont_type (m_op, m_exp2, vm->m_env, vm->m_kont));
    vm->m_ctrl = m_exp1;
}

// (rt v) E' (kprim1 op m2 E K) -> m2 E (kprim2 op v E K)
void
prim1_kont_type::eval_step (engine_type* vm, value_type* v)
{
    vm->m_env = m_env;
    vm->m_ctrl = m_exp2;
    vm->m_kont = retain_cell (vm, new prim2_kont_type (m_op, v, m_env, m_kont));
}

// (rt v) E' (kprim2 op u E K) -> (Rt w) E K
void
prim2_kont_type::eval_step (engine_type* vm, value_type* v)
{
    int lhs = 0;
    int rhs = 0;
    if (m_value->intv (lhs) && v->intv (rhs)) {
        value_type* w = nullptr;
        switch (m_op) {
        case PRIM_LT:
            w = retain_cell (vm, new value_bool_type (lhs < rhs));
            break;
        case PRIM_PLUS:
            w = retain_cell (vm, new value_int_type (lhs + rhs));
            break;
        case PRIM_MULT:
            w = retain_cell (vm, new value_int_type (lhs * rhs));
            break;
        }
        vm->m_ctrl = retain_cell (vm, new rt_type (w));
        vm->m_env = m_env;
        vm->m_kont = m_kont;
    }
    else {
        croak (vm, new expect_int_error ());
    }
}

cell_type*
prim_type::gcscan (cell_type* scan, int black)
{
    scan = scan->gccopy (m_exp1, black);
    scan = scan->gccopy (m_exp2, black);
    return scan;
}

cell_type*
prim1_kont_type::gcscan (cell_type* scan, int black)
{
    scan = scan->gccopy (m_exp2, black);
    scan = scan->gccopy (m_env, black);
    scan = scan->gccopy (m_kont, black);
    return scan;
}

cell_type*
prim2_kont_type::gcscan (cell_type* scan, int black)
{
    scan = scan->gccopy (m_value, black);
    scan = scan->gccopy (m_env, black);
    scan = scan->gccopy (m_kont, black);
    return scan;
}
