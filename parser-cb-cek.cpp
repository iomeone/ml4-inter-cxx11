#include "scanner.hpp"
#include "parser-cb-cek.hpp"

parser_cb_cek_type::parser_cb_cek_type (engine_type* vm)
{
    m_engine = vm;
}

void
parser_cb_cek_type::clear (void)
{
    m_engine->m_shunting_yard = retain_cell (m_engine, new shunting_yard_type);
}

void
parser_cb_cek_type::accept (void)
{
    m_engine->m_ctrl = m_engine->m_shunting_yard->term (1);
    m_engine->m_shunting_yard = nullptr;
}

void
parser_cb_cek_type::shift (int kind, std::string const& value)
{
    auto yy = m_engine->m_shunting_yard;
    switch (kind) {
    case SID:
        yy->shift (intern (value));
        break;
    case SINTV:
        yy->shift (std::stoi (value));
        break;
    case SBOOL:
        yy->shift (value == "true" ? 1 : 0);
        break;
    default:
        yy->shift (0); // dummy
        break;
    }
}

void
parser_cb_cek_type::prog_expr (void)
{
    auto yy = m_engine->m_shunting_yard;
    // prog : expr ';;'
    term_type* const m = yy->term (3-1);
    yy->reduce (2, m);
    accept ();
}

void
parser_cb_cek_type::prog_let (void)
{
    auto yy = m_engine->m_shunting_yard;
    // prog : 'let' ID '=' expr ';;'
    int const x = yy->lit (6-2);
    term_type* const m = yy->term (6-4);
    yy->reduce (5, retain_cell (m_engine, new decl_type (x, m)));
    accept ();
}

void
parser_cb_cek_type::prog_let_rec (void)
{
    auto yy = m_engine->m_shunting_yard;
    // prog : 'let' 'rec' ID '=' funexp ';;'
    int const x = yy->lit (7-3);
    term_type* const m = yy->term (7-5);
    yy->reduce (6, retain_cell (m_engine, new declrec_type (x, m)));
    accept ();
}

void
parser_cb_cek_type::expr_let (void)
{
    auto yy = m_engine->m_shunting_yard;
    // expr : 'let' ID '=' expr 'in' expr
    int const x = yy->lit (7-2);
    term_type* const m1 = yy->term (7-4);
    term_type* const m2 = yy->term (7-6);
    yy->reduce (6, retain_cell (m_engine, new let_type (x, m1, m2)));
}

void
parser_cb_cek_type::expr_let_rec (void)
{
    auto yy = m_engine->m_shunting_yard;
    // expr : 'let' 'rec' ID '=' funexpr 'in' expr
    int const x = yy->lit (8-3);
    term_type* const m1 = yy->term (8-5);
    term_type* const m2 = yy->term (8-7);
    yy->reduce (7, retain_cell (m_engine, new letrec_type (x, m1, m2)));
}

void
parser_cb_cek_type::expr_if (void)
{
    auto yy = m_engine->m_shunting_yard;
    // expr : 'if' expr 'then' expr 'else' expr
    term_type* const m1 = yy->term (7-2);
    term_type* const m2 = yy->term (7-4);
    term_type* const m3 = yy->term (7-6);
    yy->reduce (6, retain_cell (m_engine, new if_type (m1, m2, m3)));
}

void
parser_cb_cek_type::expr_funexpr (void)
{
    // expr : funexpr
}

void
parser_cb_cek_type::expr_pexpr (void)
{
    // expr : pexpr
}

void
parser_cb_cek_type::expr_pexpr_lt_pexpr (void)
{
    auto yy = m_engine->m_shunting_yard;
    // expr : pexpr '<' pexpr
    term_type* const m1 = yy->term (4-1);
    term_type* const m2 = yy->term (4-3);
    yy->reduce (3, retain_cell (m_engine, new prim_type (PRIM_LT, m1, m2)));
}

void
parser_cb_cek_type::funexpr (void)
{
    auto yy = m_engine->m_shunting_yard;
    // funexpr : 'fun' ID '->' expr
    int const x = yy->lit (5-2);
    term_type* const m = yy->term (5-4);
    yy->reduce (4, retain_cell (m_engine, new fun_type (x, m)));
}

void
parser_cb_cek_type::pexpr_pexpr_plus_mexpr (void)
{
    auto yy = m_engine->m_shunting_yard;
    // pexpr : pexpr '+' mexpr
    term_type* const m1 = yy->term (4-1);
    term_type* const m2 = yy->term (4-3);
    yy->reduce (3, retain_cell (m_engine, new prim_type (PRIM_PLUS, m1, m2)));
}

void
parser_cb_cek_type::pexpr_mexpr (void)
{
    // pexpr : mexpr
}

void
parser_cb_cek_type::mexpr_mexpr_times_app (void)
{
    auto yy = m_engine->m_shunting_yard;
    // mexpr : mexpr '*' app
    term_type* const m1 = yy->term (4-1);
    term_type* const m2 = yy->term (4-3);
    yy->reduce (3, retain_cell (m_engine, new prim_type (PRIM_MULT, m1, m2)));
}

void
parser_cb_cek_type::mexpr_app (void)
{
    // mexpr : app
}

void
parser_cb_cek_type::app_app_aexpr (void)
{
    auto yy = m_engine->m_shunting_yard;
    // app : app aexpr
    term_type* const m1 = yy->term (3-1);
    term_type* const m2 = yy->term (3-2);
    yy->reduce (2, retain_cell (m_engine, new app_type (m1, m2)));
}

void
parser_cb_cek_type::app_aexpr (void)
{
    // app : aexpr
}

void
parser_cb_cek_type::aexpr_id (void)
{
    auto yy = m_engine->m_shunting_yard;
    // aexpr : ID
    int const x = yy->lit (2-1);
    yy->reduce (1, retain_cell (m_engine, new ref_type (x)));
}

void
parser_cb_cek_type::aexpr_intv (void)
{
    auto yy = m_engine->m_shunting_yard;
    // aexpr : INTV
    int const c = yy->lit (2-1);
    yy->reduce (1, retain_cell (m_engine, new ilit_type (c)));
}

void
parser_cb_cek_type::aexpr_bool (void)
{
    auto yy = m_engine->m_shunting_yard;
    // aexpr : BOOLV
    int const c = yy->lit (2-1);
    yy->reduce (1, retain_cell (m_engine, new blit_type (c)));
}

void
parser_cb_cek_type::aexpr_lparen_expr_rparen (void)
{
    auto yy = m_engine->m_shunting_yard;
    // aexpr : '(' expr ')'
    term_type* const m = yy->term (4-2);
    yy->reduce (3, m);
}
