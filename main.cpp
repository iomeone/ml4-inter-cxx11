#include <cstdlib>
#include <sstream>
#include <iostream>
#include <string>
#include <stdexcept>
#include "parser.hpp"
#include "parser-cb-cek.hpp"
#include "cek-eval.hpp"

int
main (int argc, char* argv[])
{
    engine_type cek;
    cek.set_trace (false);
    scanner_type token;
    parser_cb_cek_type exp (&cek);
    for (std::string line; ; ) {
        if (! std::getline (std::cin, line) || line == "#quit;;") {
            break;
        }
        if (line == "#trace;;") {
            cek.set_trace (true);
            continue;
        }
        else if (line == "#untrace;;") {
            cek.set_trace (false);
            continue;
        }
        std::stringstream input (line);
        exp.clear ();
        if (! parse (input, token, exp)) {
            std::cout << "SyntaxError!" << std::endl;
        }
        else {
            cek.eval (std::cout);
            cek.print_result (std::cout);
            std::cout << std::endl;
        }
    }
    return EXIT_SUCCESS;
}
