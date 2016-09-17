#include <cstdlib>
#include <sstream>
#include <iostream>
#include <string>
#include <stdexcept>
#include "cek.hpp"
#include "parser.hpp"
#include "parser-cb-cek.hpp"

int
main (int argc, char* argv[])
{
    cek_type cek;
    scanner_type token;
    parser_cb_cek_type exp (&cek);
    for (std::string line;;) {
        if (! std::getline (std::cin, line)) {
            break;
        }
        if (line.find ("exit;;") != line.npos) {
            break;
        }
        std::stringstream linein (line);
        exp.clear ();
        if (! parse (linein, token, exp)) {
            std::cout << "SyntaxError!" << std::endl;
        }
        else {
            cek.eval ();
            std::cout << cek.result ()  << std::endl;
        }
    }
    return EXIT_SUCCESS;
}
