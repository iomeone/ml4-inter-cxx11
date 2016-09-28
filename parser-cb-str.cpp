#include "scanner.hpp"
#include "parser-cb-str.hpp"

std::string const&
parser_cb_str_type::str (void) const
{
    static const std::string strempty;
    return m_data.empty () ? strempty : m_data.back ();
}

void
parser_cb_str_type::clear (void)
{
    m_data.clear ();
}

void
parser_cb_str_type::shift (int kind, std::string const& value)
{
    static char const* name[] = {
        "EOF", ";;", "in", "then", "else", ")", "+", "<", "*", "=", "->",
        "rec", "let", "if", "fun", "ID", "INTV", "BOOL", "("
    };
    if (SID == kind || SINTV == kind || SBOOL == kind) {
        m_data.emplace_back (value);
    }
    else {
        m_data.emplace_back (name[kind]);
    }
}

void
parser_cb_str_type::prog_expr (void)
{
    // prog : expr ';;'
    m_data.pop_back ();
}

void
parser_cb_str_type::prog_let (void)
{
    // prog : 'let' ID '=' expr ';;'
    auto v = m_data.end () - 6;
    std::string t = "(Decl ";
    t += v[2] + " ";
    t += v[4] + ")";
    v[1] = t;
    m_data.erase (m_data.end () - 4, m_data.end ());
}

void
parser_cb_str_type::prog_let_rec (void)
{
    // prog : 'let' 'rec' ID '=' funexpr ';;'
    auto v = m_data.end () - 7;
    std::string t = "(DeclRec ";
    t += v[3] + " ";
    t += v[5] + ")";
    v[1] = t;
    m_data.erase (m_data.end () - 5, m_data.end ());
}

void
parser_cb_str_type::expr_let (void)
{
    // expr : 'let' ID '=' expr 'in' expr
    auto v = m_data.end () - 7;
    std::string t = "(Let ";
    t += v[2] + " ";
    t += v[4] + " ";
    t += v[6] + ")";
    v[1] = t;
    m_data.erase (m_data.end () - 5, m_data.end ());
}

void
parser_cb_str_type::expr_let_rec (void)
{
    // expr : 'let' 'rec' ID '=' funexpr 'in' expr
    auto v = m_data.end () - 8;
    std::string t = "(LetRec ";
    t += v[3] + " ";
    t += v[5] + " ";
    t += v[7] + ")";
    v[1] = t;
    m_data.erase (m_data.end () - 6, m_data.end ());
}

void
parser_cb_str_type::expr_if (void)
{
    // expr : 'if' expr 'then' expr 'else' expr
    auto v = m_data.end () - 7;
    std::string t = "(If ";
    t += v[2] + " ";
    t += v[4] + " ";
    t += v[6] + ")";
    v[1] = t;
    m_data.erase (m_data.end () - 5, m_data.end ());
}

void
parser_cb_str_type::expr_funexpr (void)
{
    // expr : funexpr
}

void
parser_cb_str_type::expr_pexpr (void)
{
    // expr : pexpr
}

void
parser_cb_str_type::expr_pexpr_lt_pexpr (void)
{
    // expr : pexpr '<' pexpr
    auto v = m_data.end () - 4;
    std::string t = "(Prim ";
    t += v[2] + " ";
    t += v[1] + " ";
    t += v[3] + ")";
    v[1] = t;
    m_data.erase (m_data.end () - 2, m_data.end ());
}

void
parser_cb_str_type::funexpr (void)
{
    // funexpr : 'fun' ID '->' expr
    auto v = m_data.end () - 5;
    std::string t = "(Fun ";
    t += v[2] + " ";
    t += v[4] + ")";
    v[1] = t;
    m_data.erase (m_data.end () - 3, m_data.end ());
}

void
parser_cb_str_type::pexpr_pexpr_plus_mexpr (void)
{
    // pexpr : pexpr '+' mexpr
    auto v = m_data.end () - 4;
    std::string t = "(Prim ";
    t += v[2] + " ";
    t += v[1] + " ";
    t += v[3] + ")";
    v[1] = t;
    m_data.erase (m_data.end () - 2, m_data.end ());
}

void
parser_cb_str_type::pexpr_mexpr (void)
{
    // pexpr : mexpr
}

void
parser_cb_str_type::mexpr_mexpr_times_app (void)
{
    // mexpr : mexpr '*' pexpr
    auto v = m_data.end () - 4;
    std::string t = "(Prim ";
    t += v[2] + " ";
    t += v[1] + " ";
    t += v[3] + ")";
    v[1] = t;
    m_data.erase (m_data.end () - 2, m_data.end ());
}

void
parser_cb_str_type::mexpr_app (void)
{
    // mexpr : app
}

void
parser_cb_str_type::app_app_aexpr (void)
{
    // app : app aexpr
    auto v = m_data.end () - 3;
    std::string t = "(Ap ";
    t += v[1] + " ";
    t += v[2] + ")";
    v[1] = t;
    m_data.pop_back ();
}

void
parser_cb_str_type::app_aexpr (void)
{
    // app : aexpr
}

void
parser_cb_str_type::aexpr_id (void)
{
    // aexpr : ID
    auto v = m_data.end () - 2;
    std::string t = "(Ref ";
    t += v[1] + ")";
    v[1] = t;
}

void
parser_cb_str_type::aexpr_intv (void)
{
    // aexpr : INTV
    auto v = m_data.end () - 2;
    std::string t = "(ILit ";
    t += v[1] + ")";
    v[1] = t;
}

void
parser_cb_str_type::aexpr_bool (void)
{
    // aexpr : BOOL
    auto v = m_data.end () - 2;
    std::string t = "(BLit ";
    t += v[1] + ")";
    v[1] = t;
}

void
parser_cb_str_type::aexpr_lparen_expr_rparen (void)
{
    // aexpr : '(' expr ')'
    auto v = m_data.end () - 4;
    v[1] = v[2];
    m_data.erase (m_data.end () - 2, m_data.end ());
}
