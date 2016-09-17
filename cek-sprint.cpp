#include <iostream>
#include <utility>
#include "symbol.hpp"
#include "cek.hpp"

static const char* format[] = {
    "(Error $1e)",
    "()",
    "(Decl $1x $2m)",
    "(DeclRec $1x $2m)",
    "(Let $1x $2m $3m)",
    "(LetRec $1x $2m $3m)",
    "(If $2m $3m $4m)",
    "(Ap $2m $3m)",
    "($1o $2m $3m)",
    "(Fun $1x $2d)",
    "(Ref $1x)",
    "#e$1d",
    "#$1b",
    "(Rt $2m)",
    "(Proc $1x #$2d# #$3d#)",
    "$1d",
    "$1b",
    "(Env $1x $2m $4m)",
    "(KStop $4m)",
    "(KRt #$3d# $4m)",
    "(KDecl $1x #$3d# $4m)",
    "(KDeclRec $1x #$3d# $4m)",
    "(KLet $1x $2m #$3d# $4m)",
    "(KLetRec $1x $2m #$3d# $4m)",
    "(KIf $2m $3m $4m)",
    "(KAp $2m #$3d# $4m)",
    "(KProc $1x $2m #$3d# $4m)",
    "(KPrim1 $1o $2m $4m)",
    "(KPrim2 $1o $2m $4m)",
};

static const char* primitive_operator[] = {"<", "+", "*"};

static const char* error_string[] = {
    "Success",
    "InvalidControl!",
    "InvalidContinuation!",
    "ExpectedProc!",
    "ExpectedBoolV!",
    "ExpectedIntV!",
    "UnboundVariable!",
};

void
cek_type::print_trace (void)
{
    if (m_trace) {
        std::cout << str_cek () << std::endl;
    }
}

std::string
cek_type::result (void) const
{
    if (m_cell[m_ctrl].tag == Rt) {
        return str (m_cell[m_ctrl].e2);
    }
    else if (m_cell[m_ctrl].tag == Error) {
        return str (m_ctrl);
    }
    else {
        std::string t = "unexpected CEK state\n";
        t += str_cek ();
        return t;
    }
}

std::string
cek_type::str_cek (void) const
{
    std::string t = "---\n";
    t += " C: " + str (m_ctrl) + "\n";
    t += " E: " + str (m_env) + "\n";
    t += " K: " + str (m_ctrl);
    return t;
}

std::string
cek_type::str (int const e) const
{
    int const tag = m_cell[e].tag;
    if (tag < Error || KPrim2 < tag) {
        return "(Unknown)";
    }
    std::string const src = format[tag];
    std::string out;
    for (std::size_t i = 0; i < src.size (); ++i) {
        if ('$' != src[i]) {
            out.push_back (src[i]);
            continue;
        }
        char const slot = src[++i];
        char const look = src[++i];
        int const v = '1' == slot ? m_cell[e].e1
                    : '2' == slot ? m_cell[e].e2
                    : '3' == slot ? m_cell[e].e3
                    :               m_cell[e].e4;
        switch (look) {
        case 'd':
            out += std::to_string (v);
            break;
        case 'b':
            out += v ? "true" : "false";
            break;
        case 'x':
            out += symbol_table::instance ().name (v);
            break;
        case 'm':
            out += str (v);
            break;
        case 'o':
            out += Lt <= v && v <= Mult ? primitive_operator[v]
                 : "?";
            break;
        case 'e':
            out += Success <= v && v <= UnboundVariable ? error_string[v]
                 : "RuntimeError!";
            break;
        }
    }
    return std::move (out);
}
