#pragma once
#include <string>
#include <unordered_map>
#include <vector>

typedef int symbol;

class symbol_table {
public:
    static symbol_table& instance (void);
    symbol intern (std::string const& str);
    std::string name (symbol id);

private:
    symbol_table () : m_index (), m_dict () {}
    std::unordered_map<std::string,symbol> m_index;
    std::vector<std::string> m_dict;
};

symbol intern (std::string const& str);
