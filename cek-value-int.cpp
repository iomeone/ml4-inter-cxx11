#include "cek-eval.hpp"
#include "cek-value-int.hpp"

// integer variant selector for value of evaluating result

value_int_type::value_int_type (int i)
    : value_type (), m_value (i)
{
}

// poor man's pattern matching
bool
value_int_type::intv (int& i) const
{
    i = m_value;
    return true;
}

// show print style
void
value_int_type::print (std::ostream& out) const
{
    out << m_value;
}

// show dump style
void
value_int_type::dump (std::ostream& out) const
{
    printformat (out, "(intv $1d)", m_value);
}

// use default garbage collection copying due to no child cell
