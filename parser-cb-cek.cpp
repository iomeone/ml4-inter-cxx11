#include "scanner.hpp"
#include "parser-cb-cek.hpp"

parser_cb_cek_type::parser_cb_cek_type (cek_type* cek)
{
    m_cek = cek;
}

void
parser_cb_cek_type::clear (void)
{
    m_cek->begin_ctrl ();
}

void
parser_cb_cek_type::shift (int kind, std::string const& value)
{
    switch (kind) {
    case SID:
        m_cek->ctrl_shift (intern (value));
        break;
    case SINTV:
        m_cek->ctrl_shift (std::stoi (value));
        break;
    case SBOOL:
        m_cek->ctrl_shift (value == "true" ? 1 : 0);
        break;
    default:
        m_cek->ctrl_shift (0); // dummy
        break;
    }
}

void
parser_cb_cek_type::prog_expr (void)
{
    // prog : expr ';;'
    int const exp = m_cek->ctrl_exp (3-1);
    m_cek->ctrl_reduce (2, exp);
    m_cek->end_ctrl ();
}

void
parser_cb_cek_type::prog_let (void)
{
    // prog : 'let' ID '=' expr ';;'
    int const x = m_cek->ctrl_lit (6-2);
    int const exp = m_cek->ctrl_exp (6-4);
    m_cek->ctrl_reduce (5, Decl, x, exp, 0, 0);
    m_cek->end_ctrl ();
}

void
parser_cb_cek_type::prog_let_rec (void)
{
    // prog : 'let' 'rec' ID '=' exp ';;'
    int const y = m_cek->ctrl_lit (7-3);
    int const fn = m_cek->ctrl_exp (7-5);
    m_cek->ctrl_reduce (6, DeclRec, y, fn, 0, 0);
    m_cek->end_ctrl ();
}

void
parser_cb_cek_type::expr_let (void)
{
    // expr : 'let' ID '=' expr 'in' expr
    int const x = m_cek->ctrl_lit (7-2);
    int const e1 = m_cek->ctrl_exp (7-4);
    int const e2 = m_cek->ctrl_exp (7-6);
    m_cek->ctrl_reduce (6, Let, x, e1, e2, 0);
}

void
parser_cb_cek_type::expr_let_rec (void)
{
    // expr : 'let' 'rec' ID '=' expr 'in' expr
    int const x = m_cek->ctrl_lit (8-3);
    int const e1 = m_cek->ctrl_exp (8-5);
    int const e2 = m_cek->ctrl_exp (8-7);
    m_cek->ctrl_reduce (7, LetRec, x, e1, e2, 0);
}

void
parser_cb_cek_type::expr_if (void)
{
    // expr : 'if' expr 'then' expr 'else' expr
    int const e1 = m_cek->ctrl_exp (7-2);
    int const e2 = m_cek->ctrl_exp (7-4);
    int const e3 = m_cek->ctrl_exp (7-6);
    m_cek->ctrl_reduce (6, If, 0, e1, e2, e3);
}

void
parser_cb_cek_type::expr_fun (void)
{
    // expr : 'fun' ID '->' expr
    int const x = m_cek->ctrl_lit (5-2);
    int const exp = m_cek->ctrl_exp (5-4);
    m_cek->ctrl_reduce (4, Fun, x, exp, 0, 0);
}

void
parser_cb_cek_type::expr_pexpr (void)
{
    // expr : pexpr
}

void
parser_cb_cek_type::expr_pexpr_lt_pexpr (void)
{
    // expr : pexpr '<' pexpr
    int const exp1 = m_cek->ctrl_exp (4-1);
    int const exp2 = m_cek->ctrl_exp (4-3);
    m_cek->ctrl_reduce (3, Prim, Lt, exp1, exp2, 0);
}

void
parser_cb_cek_type::pexpr_pexpr_plus_mexpr (void)
{
    // pexpr : pexpr '+' mexpr
    int const exp1 = m_cek->ctrl_exp (4-1);
    int const exp2 = m_cek->ctrl_exp (4-3);
    m_cek->ctrl_reduce (3, Prim, Plus, exp1, exp2, 0);
}

void
parser_cb_cek_type::pexpr_mexpr (void)
{
    // pexpr : mexpr
}

void
parser_cb_cek_type::mexpr_mexpr_times_app (void)
{
    // mexpr : mexpr '*' app
    int const exp1 = m_cek->ctrl_exp (4-1);
    int const exp2 = m_cek->ctrl_exp (4-3);
    m_cek->ctrl_reduce (3, Prim, Mult, exp1, exp2, 0);
}

void
parser_cb_cek_type::mexpr_app (void)
{
    // mexpr : app
}

void
parser_cb_cek_type::app_app_aexpr (void)
{
    // app : app aexpr
    int const exp1 = m_cek->ctrl_exp (3-1);
    int const exp2 = m_cek->ctrl_exp (3-2);
    m_cek->ctrl_reduce (2, Ap, 0, exp1, exp2, 0);
}

void
parser_cb_cek_type::app_aexpr (void)
{
    // app : aexpr
}

void
parser_cb_cek_type::aexpr_id (void)
{
    // aexpr : ID
    int const c = m_cek->ctrl_lit (2-1);
    m_cek->ctrl_reduce (1, Ref, c, 0, 0, 0);
}

void
parser_cb_cek_type::aexpr_intv (void)
{
    // aexpr : INTV
    int const c = m_cek->ctrl_lit (2-1);
    m_cek->ctrl_reduce (1, ILit, c, 0, 0, 0);
}

void
parser_cb_cek_type::aexpr_bool (void)
{
    // aexpr : BOOLV
    int const c = m_cek->ctrl_lit (2-1);
    m_cek->ctrl_reduce (1, BLit, c, 0, 0, 0);
}

void
parser_cb_cek_type::aexpr_lparen_expr_rparen (void)
{
    // aexpr : '(' expr ')'
    int const exp = m_cek->ctrl_exp (4-2);
    m_cek->ctrl_reduce (3, exp);
}
