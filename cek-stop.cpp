#include "cek-eval.hpp"
#include "cek-stop.hpp"

// stop continuation marker

stop_kont_type::stop_kont_type ()
    : kont_type (nullptr, nullptr)
{
}

// show print style
void
stop_kont_type::print (std::ostream& out) const
{
    out << "(kstop)";
}

// show dump style
void
stop_kont_type::dump (std::ostream& out) const
{
    out << "(kstop)";
}

// evaluation

// C E (kstop) -> C E (kstop)
void
stop_kont_type::eval_step (engine_type* vm, value_type* v)
{
}

// use default garbage collection copying due to no child cell
