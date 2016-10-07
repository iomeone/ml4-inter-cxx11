#include "cek-eval.hpp"
#include "cek-value-bool.hpp"

// boolean variant selector for value of evaluating result

value_bool_type::value_bool_type (bool b)
    : value_type (), m_value (b)
{
}

// poor man's pattern matching
bool
value_bool_type::boolv (bool& b) const
{
    b = m_value;
    return true;
}

// show print style
void
value_bool_type::print (std::ostream& out) const
{
    printformat (out, "$1b", m_value);
}

// show dump style
void
value_bool_type::dump (std::ostream& out) const
{
    printformat (out, "(boolv $1b)", m_value);
}

// use default garbage collection copying due to no child cell
