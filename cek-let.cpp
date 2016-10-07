#include "cek-eval.hpp"
#include "cek-let.hpp"
#include "cek-let-kont.hpp"

// let expression term `let x = exp1 in exp2`
//
//     function (x, exp1, exp2, env, kont) {  /* let_type */
//       eval (exp1, env, function (value) {  /* let_kont_type */
//         eval (exp2, new env_type (x, value, env), kont);
//       });
//     }

let_type::let_type (symbol x, term_type* m1, term_type* m2)
    : term_type (), m_id (x), m_exp1 (m1), m_exp2 (m2)
{
}

let_kont_type::let_kont_type (symbol x, term_type* m2, env_type* e, kont_type* k)
    : kont_type (e, k), m_id (x), m_exp2 (m2)
{
}

// show print style
void
let_type::print (std::ostream& out) const
{
    printformat (out, "(let $1x = $2m in $3m)", m_id, m_exp1, m_exp2);
}

void
let_kont_type::print (std::ostream& out) const
{
    printformat (out, "(klet $1x $2m #$3p# $4m)", m_id, m_exp2, m_env, m_kont);
}

// show dump style
void
let_type::dump (std::ostream& out) const
{
    printformat (out, "(let $1x $2p $3p)", m_id, m_exp1, m_exp2);
}

void
let_kont_type::dump (std::ostream& out) const
{
    printformat (out, "(klet $1x $2p $3p $4p)", m_id, m_exp2, m_env, m_kont);
}

// evaluation

// (let x m1 m2) E K -> m1 E (klet x m2 E K)
void
let_type::eval_step (engine_type* vm)
{
    vm->m_kont = retain_cell (vm,
        new let_kont_type (m_id, m_exp2, vm->m_env, vm->m_kont));
    vm->m_ctrl = m_exp1;
}

// (rt v) E' (klet x m2 E K) -> m2 (env x v E) K
void
let_kont_type::eval_step (engine_type* vm, value_type* v)
{
    vm->m_env = retain_cell (vm, new env_type (m_id, v, vm->m_env));
    vm->m_ctrl = m_exp2;
    vm->m_kont = m_kont;
}

// garbage collection copying
cell_type*
let_type::gcscan (cell_type* scan, int black)
{
    scan = scan->gccopy (m_exp1, black);
    scan = scan->gccopy (m_exp2, black);
    return scan;
}

cell_type*
let_kont_type::gcscan (cell_type* scan, int black)
{
    scan = scan->gccopy (m_exp2, black);
    scan = scan->gccopy (m_env, black);
    scan = scan->gccopy (m_kont, black);
    return scan;
}
