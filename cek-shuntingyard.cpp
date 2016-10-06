#include "cek-eval.hpp"

shunting_yard_type::shunting_yard_type ()
    : cell_type (), m_data ()
{
}

void
shunting_yard_type::print (std::ostream& out) const
{
    printformat (out, "(shunting-yard $1d)", m_data.size ());
}

void
shunting_yard_type::dump (std::ostream& out) const
{
    print (out);
}

void
shunting_yard_type::shift (int v)
{
    m_data.emplace_back (v, nullptr);
}

int
shunting_yard_type::lit (int i) const
{
    return m_data[m_data.size () - i].first;
}

term_type*
shunting_yard_type::term (int i) const
{
    return m_data[m_data.size () - i].second;
}

void
shunting_yard_type::reduce (int rhs_len, term_type* term)
{
    m_data[m_data.size () - rhs_len].second = term;
    if (rhs_len > 1) {
        m_data.erase (m_data.end () - rhs_len + 1, m_data.end ());
    }
}

cell_type*
shunting_yard_type::gcscan (cell_type* scan, int black)
{
    for (auto it : m_data) {
        scan = scan->gccopy (it.second, black);
    }
    return scan;
}
