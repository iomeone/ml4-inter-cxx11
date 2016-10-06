#include "cek-eval.hpp"
#include "cek-value-bool.hpp"

value_bool_type::value_bool_type (bool b)
    : value_type (), m_value (b)
{
}

bool
value_bool_type::boolv (bool& b) const
{
    b = m_value;
    return true;
}

void
value_bool_type::print (std::ostream& out) const
{
    printformat (out, "$1b", m_value);
}

void
value_bool_type::dump (std::ostream& out) const
{
    printformat (out, "(boolv $1b)", m_value);
}
