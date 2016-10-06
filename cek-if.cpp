#include "cek-eval.hpp"
#include "cek-if.hpp"
#include "cek-if-kont.hpp"
#include "cek-error.hpp"

if_type::if_type (term_type* m1, term_type* m2, term_type* m3)
    : term_type (), m_exp1 (m1), m_exp2 (m2), m_exp3 (m3)
{
}

if_kont_type::if_kont_type (term_type* m2, term_type* m3, env_type* e, kont_type* k)
    : kont_type (e, k), m_exp2 (m2), m_exp3 (m3)
{
}

void
if_type::print (std::ostream& out) const
{
    printformat (out, "(if $2m then $3m else $4m)", 0, m_exp1, m_exp2, m_exp3);
}

void
if_kont_type::print (std::ostream& out) const
{
    printformat (out, "(kif $2m $3m #$4p# $5m)", 0, m_exp2, m_exp3, m_env, m_kont);
}

void
if_type::dump (std::ostream& out) const
{
    printformat (out, "(if $2p $3p $4p)", 0, m_exp1, m_exp2, m_exp3);
}

void
if_kont_type::dump (std::ostream& out) const
{
    printformat (out, "(kif $2p $3p $4p $5p)", 0, m_exp2, m_exp3, m_env, m_kont);
}

// (if m1 m2 m3) E K -> m1 E (kif m2 m3 E K)
void
if_type::eval_step (engine_type* vm)
{
    vm->m_kont = retain_cell (vm,
        new if_kont_type (m_exp2, m_exp3, vm->m_env, vm->m_kont));
    vm->m_ctrl = m_exp1;
}

// (rt (boolv true))  E' (kif m2 m3 E K) -> m2 E K
// (rt (boolv false)) E' (kif m2 m3 E K) -> m3 E K
// (rt _) E' (kif m2 m3 E K) -> (rt type_error) E' protect
void
if_kont_type::eval_step (engine_type* vm, value_type* v)
{
    bool value = false;
    if (v->boolv (value)) {
        vm->m_env = m_env;
        vm->m_ctrl = value ? m_exp2 : m_exp3;
        vm->m_kont = m_kont;
    }
    else {
        croak (vm, retain_cell (vm, new expect_bool_error ()));
    }
}

cell_type*
if_type::gcscan (cell_type* scan, int black)
{
    scan = scan->gccopy (m_exp1, black);
    scan = scan->gccopy (m_exp2, black);
    scan = scan->gccopy (m_exp3, black);
    return scan;
}

cell_type*
if_kont_type::gcscan (cell_type* scan, int black)
{
    scan = scan->gccopy (m_exp2, black);
    scan = scan->gccopy (m_exp3, black);
    scan = scan->gccopy (m_env, black);
    scan = scan->gccopy (m_kont, black);
    return scan;
}
