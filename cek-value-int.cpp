#include "cek-eval.hpp"
#include "cek-value-int.hpp"

value_int_type::value_int_type (int i)
    : value_type (), m_value (i)
{
}

bool
value_int_type::intv (int& i) const
{
    i = m_value;
    return true;
}

void
value_int_type::print (std::ostream& out) const
{
    out << m_value;
}

void
value_int_type::dump (std::ostream& out) const
{
    printformat (out, "(intv $1d)", m_value);
}
