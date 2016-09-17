#pragma once
#include <istream>
#include <string>
#include <unordered_map>

enum {
    SEOF, SSEMISEMI, SIN, STHEN, SELSE, SRPAREN, SPLUS, SLT, SMULT,
    SEQ, SARROW, SREC, SLET, SIF, SFUN, SID, SINTV, SBOOL, SLPAREN,
};

class scanner_type {
public:
    scanner_type ();
    bool next (std::istream& cin);
    int kind (void) const { return m_kind; }
    std::string const& value (void) const { return m_value; }

private:
    void init_keyword (void);
    bool success (void);
    bool skip_comment (std::istream& cin);
    std::unordered_map<std::string,int> m_keyword;
    int m_kind;
    std::string m_value;
};
