#include "cek-eval.hpp"
#include "cek-app.hpp"
#include "cek-app1-kont.hpp"
#include "cek-app2-kont.hpp"
#include "cek-error.hpp"

app_type::app_type (term_type* m1, term_type* m2)
    : term_type (), m_exp1 (m1), m_exp2 (m2)
{
}

app1_kont_type::app1_kont_type (term_type* m2, env_type* e, kont_type* k)
    : kont_type (e, k), m_exp2 (m2)
{
}

app2_kont_type::app2_kont_type (symbol x, term_type* m, env_type* e, kont_type* k)
    : kont_type (e, k), m_id (x), m_exp (m)
{
}

void
app_type::print (std::ostream& out) const
{
    printformat (out, "($2m $3m)", 0, m_exp1, m_exp2);
}

void
app1_kont_type::print (std::ostream& out) const
{
    printformat (out, "(kapp1 $2m #$3p# $4m)", 0, m_exp2, m_env, m_kont);
}

void
app2_kont_type::print (std::ostream& out) const
{
    printformat (out, "(kapp2 $1x $2m #$3p# $4m)", m_id, m_exp, m_env, m_kont);
}

void
app_type::dump (std::ostream& out) const
{
    printformat (out, "(app $2p $3p)", 0, m_exp1, m_exp2);
}

void
app1_kont_type::dump (std::ostream& out) const
{
    printformat (out, "(kapp1 $2p $3p $4p)", 0, m_exp2, m_env, m_kont);
}

void
app2_kont_type::dump (std::ostream& out) const
{
    printformat (out, "(kapp2 $1x $2p $3p $4p)", m_id, m_exp, m_env, m_kont);
}

// (app m1 m2) E K -> m1 E (kapp1 m2 E K)
void
app_type::eval_step (engine_type* vm)
{
    vm->m_kont = retain_cell (vm, new app1_kont_type (m_exp2, vm->m_env, vm->m_kont));
    vm->m_ctrl = m_exp1;
}

// (rt (closure x m E)) E' (kapp1 m2 E2 K) -> m2 E2 (kapp2 x m E K)
void
app1_kont_type::eval_step (engine_type* vm, value_type* v)
{
    symbol x;
    term_type* m;
    env_type* e;
    if (v->closure (x, m, e)) {
        vm->m_kont = retain_cell (vm, new app2_kont_type (x, m, e, m_kont));
        vm->m_env = m_env;
        vm->m_ctrl = m_exp2;
    }
    else {
        croak (vm, retain_cell (vm, new expect_closure_error ()));
    }
}

// (rt v) E' (kap2 x m E K) -> m (env x v E) K
void
app2_kont_type::eval_step (engine_type* vm, value_type* v)
{
    vm->m_env = retain_cell (vm, new env_type (m_id, v, m_env));
    vm->m_ctrl = m_exp;
    vm->m_kont = m_kont;
}

cell_type*
app_type::gcscan (cell_type* scan, int black)
{
    scan = scan->gccopy (m_exp1, black);
    scan = scan->gccopy (m_exp2, black);
    return scan;
}

cell_type*
app1_kont_type::gcscan (cell_type* scan, int black)
{
    scan = scan->gccopy (m_exp2, black);
    scan = scan->gccopy (m_env, black);
    scan = scan->gccopy (m_kont, black);
    return scan;
}

cell_type*
app2_kont_type::gcscan (cell_type* scan, int black)
{
    scan = scan->gccopy (m_exp, black);
    scan = scan->gccopy (m_env, black);
    scan = scan->gccopy (m_kont, black);
    return scan;
}
