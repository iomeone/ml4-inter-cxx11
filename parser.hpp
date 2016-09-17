#pragma once
#include <istream>
#include "scanner.hpp"
#include "parser-cb-base.hpp"

bool parse (std::istream& cin, scanner_type& token, parser_callback_base& cb);
