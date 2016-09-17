#include "scanner.hpp"

// Lexical token scanner
//
//  "else"   SELSE
//  "false"  SBOOL
//  "fun"    SFUN
//  "if"     SIF
//  "in"     SIN
//  "let"    SLET
//  "rec"    SREC
//  "then"   STHEN
//  "true"   SBOOL
//  ";;"     SSEMISEMI
//  "="      SEQ
//  "->"     SARROW
//  "<"      SLT
//  "+"      SPLUS
//  "*"      SMULT
//  "("      SLPAREN
//  ")"      SRPAREN
//  [-]?[0-9]+       SINTV
//  [a-z][a-z0-9_]*  SID

static const char CODE[] =
  // @ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_ !"#$%&'()*+,-./0123456789:;<=>?
    "@@@@@@@@@NN@@N@@@@@@@@@@@@@@@@@@N@@@@@@@FGHI@J@@DDDDDDDDDD@KLMC@"
  // @ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~
    "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@B@EEEEEEEEEEEEEEEEEEEEEEEEEE@@@@@";
static const short BASE[] = {
    0, -4, 10, 11, 16, 17, 18, 19, 18, 12, 24, 25, 26, 27, 28
};
static const unsigned short RULE[] = {
    0x0201, 0x0301, 0x0401, 0x0501, 0x0601, 0x0701, 0x0801, 0x0901,
    0x0a01, 0x0b01, 0x0101, 0x1002, 0x0f03, 0x0303, 0x0202, 0x0303,
    0x0303, 0x1204, 0x0505, 0x0806, 0x0607, 0x0e08, 0x0208, 0x0d09,
    0x0c04, 0x070a, 0x090b, 0x000c, 0x010d, 0x0a0e,
};

scanner_type::scanner_type ()
{
    init_keyword ();
}

static bool
forward (int const state, int const code, int& dest)
{
    static const int ncheck = sizeof (RULE) / sizeof (RULE[0]);
    int const i = BASE[state] + code;
    if (0 <= i && i < ncheck && state == (RULE[i] & 0xff)) {
        dest = RULE[i] >> 8;
        return true;
    }
    return false;
}

bool
scanner_type::next (std::istream& cin)
{
    enum { ERROR = 0, START = 1, OK = 1 };
    m_value.clear ();
    for (int next_state = START; next_state > 0; ) {
        int const state = next_state;
        next_state = ERROR;
        if (cin.eof ()) {
            if (START == state) {
                m_kind = SEOF;
                return success ();
            }
        }
        else {
            int const ch = cin.peek ();
            int const code = 0 < ch && ch < 127 ? CODE[ch] - '@' : 0;
            if (forward (state, code, next_state)) {
                if (START != next_state) {
                    m_value.push_back (ch);
                }
                cin.get ();
                continue;
            }
        }
        if (forward (state, OK, m_kind)) {
            if (m_kind > 0) {
                return success ();
            }
            next_state = skip_comment (cin) ? START : ERROR;
        }
    }
    return false;
}

void
scanner_type::init_keyword (void)
{
    m_keyword["else"] = SELSE;
    m_keyword["false"] = SBOOL;
    m_keyword["fun"] = SFUN;
    m_keyword["if"] = SIF;
    m_keyword["in"] = SIN;
    m_keyword["let"] = SLET;
    m_keyword["rec"] = SREC;
    m_keyword["then"] = STHEN;
    m_keyword["true"] = SBOOL;
}

bool
scanner_type::success (void)
{
    if (SID == m_kind) {
        auto it = m_keyword.find (m_value);
        if (it != m_keyword.end ()) {
            m_kind = it->second;
        }
    }
    return true;
}

bool
scanner_type::skip_comment (std::istream& cin)
{
    m_value.clear ();
    for (int level = 1, state = 1; level > 0; ) {
        int const ch = cin.get ();
        if (cin.eof () == ch) {
            return false;
        }
        else if (1 == state) {
            state = '*' == ch ? 2 : '(' == ch ? 3 : 1;
        }
        else if (2 == state) {
            if (')' == ch) {
                --level;    // "*)"
            }
            state = 1;
        }
        else if (3 == state) {
            if ('*' == ch) {
                ++level;    // "(*"
            }
            state = 1;
        }
    }
    return true;
}
