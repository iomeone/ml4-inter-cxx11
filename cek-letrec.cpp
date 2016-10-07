#include "cek-eval.hpp"
#include "cek-letrec.hpp"
#include "cek-letrec-kont.hpp"

// let rec expression term `let rec x = exp1 in exp2`,
// where exp1 must be function term.
//
//     function (x, exp1, exp2, env, kont) {  /* letrec_type */
//       env1 = new env_type (x, dummy, env);
//       eval (exp1, env1, function (value) { /* letrec_kont_type */
//         env1.value = value;
//         eval (exp2, env1, kont);
//       });
//     }

letrec_type::letrec_type (symbol x, term_type* m1, term_type* m2)
    : term_type (), m_id (x), m_exp1 (m1), m_exp2 (m2)
{
}

letrec_kont_type::letrec_kont_type (symbol x, term_type* m2, env_type* e, kont_type* k)
    : kont_type (e, k), m_id (x), m_exp2 (m2)
{
}

// show print style
void
letrec_type::print (std::ostream& out) const
{
    printformat (out, "(let rec $1x = $2m in $3m)", m_id, m_exp1, m_exp2);
}

void
letrec_kont_type::print (std::ostream& out) const
{
    printformat (out, "(kletrec $1x $2m #$3p# $4m)", m_id, m_exp2, m_env, m_kont);
}

// show dump style
void
letrec_type::dump (std::ostream& out) const
{
    printformat (out, "(letrec $1x $2p $3p)", m_id, m_exp1, m_exp2);
}

void
letrec_kont_type::dump (std::ostream& out) const
{
    printformat (out, "(kletrec $1x $2p $3p $4p)", m_id, m_exp2, m_env, m_kont);
}


// evaluation

// (let x m1 m2) E K -> m1 E1=(x DUMMY E) (kletrec x m2 E1 K)
void
letrec_type::eval_step (engine_type* vm)
{
    vm->m_env = retain_cell (vm, new env_type (m_id, nullptr, vm->m_env));
    vm->m_kont = retain_cell (vm, new letrec_kont_type (m_id, m_exp2, vm->m_env, vm->m_kont));
    vm->m_ctrl = m_exp1;
}

// (rt v) E' (kletrec x m2 E1=(x DUMMY E) K) -> m2 E1=(env x v E) K
void
letrec_kont_type::eval_step (engine_type* vm, value_type* v)
{
    vm->m_env = m_env;
    vm->m_env->set_value (v);
    vm->m_ctrl = m_exp2;
    vm->m_kont = m_kont;
}

// garbage collection copying
cell_type*
letrec_type::gcscan (cell_type* scan, int black)
{
    scan = scan->gccopy (m_exp1, black);
    scan = scan->gccopy (m_exp2, black);
    return scan;
}

cell_type*
letrec_kont_type::gcscan (cell_type* scan, int black)
{
    scan = scan->gccopy (m_exp2, black);
    scan = scan->gccopy (m_env, black);
    scan = scan->gccopy (m_kont, black);
    return scan;
}
