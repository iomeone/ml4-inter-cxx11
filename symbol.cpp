#include "symbol.hpp"

symbol_table&
symbol_table::instance (void)
{
    static symbol_table obj;
    return obj;
}

symbol
symbol_table::intern (std::string const& str)
{
    auto it = m_index.find (str);
    if (it == m_index.end ()) {
        symbol const id = m_dict.size ();
        m_dict.emplace_back (str);
        m_index[str] = id;
        return id;
    }
    return it->second;
}

std::string
symbol_table::name (symbol id)
{
    int const dict_size = m_dict.size ();
    if (0 <= id && id < dict_size) {
        return m_dict[id];
    }
    return "-";
}

symbol
intern (std::string const& str)
{
    return symbol_table::instance ().intern (str);
}

std::string
symbol_name (symbol const id)
{
    return symbol_table::instance ().name (id);
}
