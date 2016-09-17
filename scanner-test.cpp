#include "scanner.hpp"
#include "taptests.hpp"
#include <sstream>

void test1 (test::simple& ts);

int main (int argc, char* argv[])
{
    test::simple ts;
    test1 (ts);
    return ts.done_testing ();
}

//    SEOF, SSEMISEMI, SIN, STHEN, SELSE, SRPAREN, SPLUS, SLT, SMULT,
//    SEQ, SARROW, SREC, SLET, SIF, SFUN, SID, SINTV, SBOOL, SLPAREN,

void test1 (test::simple& ts)
{
    scanner_type tok;
    std::istringstream cin (
        ";;in then else)+<*=->rec let if fun i if0 0 -1 123 true false (");
    ts.ok (tok.next (cin), "next");
    ts.ok (tok.kind () == SSEMISEMI, ";;");
    ts.ok (tok.next (cin), "next");
    ts.ok (tok.kind () == SIN, "in");
    ts.ok (tok.next (cin), "next");
    ts.ok (tok.kind () == STHEN, "then");
    ts.ok (tok.next (cin), "next");
    ts.ok (tok.kind () == SELSE, "else");
    ts.ok (tok.next (cin), "next");
    ts.ok (tok.kind () == SRPAREN, ")");
    ts.ok (tok.next (cin), "next");
    ts.ok (tok.kind () == SPLUS, "+");
    ts.ok (tok.next (cin), "next");
    ts.ok (tok.kind () == SLT, "<");
    ts.ok (tok.next (cin), "next");
    ts.ok (tok.kind () == SMULT, "*");
    ts.ok (tok.next (cin), "next");
    ts.ok (tok.kind () == SEQ, "=");
    ts.ok (tok.next (cin), "next");
    ts.ok (tok.kind () == SARROW, "->");
    ts.ok (tok.next (cin), "next");
    ts.ok (tok.kind () == SREC, "rec");
    ts.ok (tok.next (cin), "next");
    ts.ok (tok.kind () == SLET, "let");
    ts.ok (tok.next (cin), "next");
    ts.ok (tok.kind () == SIF, "if");
    ts.ok (tok.next (cin), "next");
    ts.ok (tok.kind () == SFUN, "fun");
    ts.ok (tok.next (cin), "next");
    ts.ok (tok.kind () == SID, "ID");
    ts.ok (tok.value () == "i", "i");
    ts.ok (tok.next (cin), "next");
    ts.ok (tok.kind () == SID, "ID");
    ts.ok (tok.value () == "if0", "if0");
    ts.ok (tok.next (cin), "next");
    ts.ok (tok.kind () == SINTV, "INTV");
    ts.ok (tok.value () == "0", "0");
    ts.ok (tok.next (cin), "next");
    ts.ok (tok.kind () == SINTV, "INTV");
    ts.ok (tok.value () == "-1", "-1");
    ts.ok (tok.next (cin), "next");
    ts.ok (tok.kind () == SINTV, "INTV");
    ts.ok (tok.value () == "123", "123");
    ts.ok (tok.next (cin), "next");
    ts.ok (tok.kind () == SBOOL, "SBOOL");
    ts.ok (tok.value () == "true", "true");
    ts.ok (tok.next (cin), "next");
    ts.ok (tok.kind () == SBOOL, "SBOOL");
    ts.ok (tok.value () == "false", "false");
    ts.ok (tok.next (cin), "next");
    ts.ok (tok.kind () == SLPAREN, "(");
    ts.ok (tok.next (cin), "next");
    ts.ok (tok.kind () == SEOF, "EOF");
}
