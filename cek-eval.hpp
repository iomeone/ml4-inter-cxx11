#pragma once
#include <vector>
#include <ostream>
#include <utility>
#include "symbol.hpp"

enum { PRIM_LT, PRIM_PLUS, PRIM_MULT };

#include "cek-cell.hpp"

class engine_type;
class term_type;
class env_type;

#include "cek-value.hpp"
#include "cek-ctrl.hpp"
#include "cek-term.hpp"
#include "cek-env.hpp"
#include "cek-kont.hpp"
#include "cek-shuntingyard.hpp"
#include "cek-engine.hpp"

#include "cek-decl.hpp"
#include "cek-declrec.hpp"
#include "cek-let.hpp"
#include "cek-letrec.hpp"
#include "cek-if.hpp"
#include "cek-fun.hpp"
#include "cek-app.hpp"
#include "cek-prim.hpp"
#include "cek-ref.hpp"
#include "cek-blit.hpp"
#include "cek-ilit.hpp"

template<class CELL>
CELL* retain_cell (engine_type* vm, CELL* ptr)
{
    vm->retain (ptr);
    return ptr;
}
