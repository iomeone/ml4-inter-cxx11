#include "cek-eval.hpp"

// tool of building an abstract syntax tree.
// It is suitable to work with bottom-up parser.
//
//     input:  3 + 5 * 6
//     output: ('+' 3 ('*' 5 6))
//
//     shunting_yard_type* yy = vm.m_shunting_yard;
//     yy->clear ();
//     yy->shift (3);
//     yy->reduce (1, retain_cell (vm, new ilit_type (yy->lit (2-1))));
//     yy->shift ('+');
//     yy->shift (5);
//     yy->reduce (1, retain_cell (vm, new ilit_type (yy->lit (2-1))));
//     yy->shift ('*');
//     yy->shift (6);
//     yy->reduce (1, retain_cell (vm, new ilit_type (yy->lit (2-1))));
//     yy->reduce (3, retain_cell (vm, 
//         new prim_type (PRIM_MULT, yy->term (4-1), yy->term (4-3))));
//     yy->reduce (3, retain_cell (vm,
//         new prim_type (PRIM_ADD, yy->term (4-1), yy->term (4-3))));
//     vm.m_ctrl = yy->term (1);
//
// On reduce, get a token value or a syntax sub-tree,
// and push back a constructed syntax tree. 

shunting_yard_type::shunting_yard_type ()
    : cell_type (), m_data ()
{
}

// show print style
void
shunting_yard_type::print (std::ostream& out) const
{
    printformat (out, "(shunting-yard $1d)", m_data.size ());
}

// show dump style
void
shunting_yard_type::dump (std::ostream& out) const
{
    print (out);
}

// On shift, push back a token value.
// In this version, token value is only available as an integer.
void
shunting_yard_type::shift (int v)
{
    m_data.emplace_back (v, nullptr);
}

// On reduce, get a token literal at the reverse index i.
// For instance,
//
//     letrec-expr : let rec ID  =   funexpr in  expr
//                   ^^^ ^^^ ^^  ^   ^^^^^^^ ^^  ^^^^
//              i is 7   6   5   4   3       2   1
//                or 8-1 8-2 8-3 8-4 8-5     8-6 8-7
//
// `parser-cb-cek.cpp` uses later N-i notation to readability.
//
//     symbol id = yy.lit (8-3);
//     term_type* exp1 = yy.term (8-5);
//     term_type* exp2 = yy.term (8-7);
//
int
shunting_yard_type::lit (int i) const
{
    return m_data[m_data.size () - i].first;
}

// On reduce, get a term at the reverse index i.
term_type*
shunting_yard_type::term (int i) const
{
    return m_data[m_data.size () - i].second;
}

// On reduce, drop right hand side and push a term.
void
shunting_yard_type::reduce (int rhs_len, term_type* term)
{
    m_data[m_data.size () - rhs_len].second = term;
    if (rhs_len > 1) {
        m_data.erase (m_data.end () - rhs_len + 1, m_data.end ());
    }
}

// garbage collection copying
cell_type*
shunting_yard_type::gcscan (cell_type* scan, int black)
{
    for (auto it : m_data) {
        scan = scan->gccopy (it.second, black);
    }
    return scan;
}
