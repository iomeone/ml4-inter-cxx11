#include <cstdlib>
#include <iostream>
#include <sstream>
#include "scanner.hpp"
#include "parser.hpp"
#include "parser-cb-str.hpp"

int
main (int argc, char* argv[])
{
    scanner_type scanner;
    parser_cb_str_type output;
    std::cout << "exit;; to quit." << std::endl;
    for (std::string line;;) {
        std::cout << "> "; std::cout.flush ();
        if (! std::getline (std::cin, line)) break;
        if (line == "exit;;") break;
        std::istringstream input (line);
        output.clear ();
        if (parse (input, scanner, output)) {
            std::cout << "ok " << output.str () << std::endl;
        }
        else {
            std::cout << "not ok" << std::endl;
        }
    }
    return EXIT_SUCCESS;
}
