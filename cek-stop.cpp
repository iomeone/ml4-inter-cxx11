#include "cek-eval.hpp"
#include "cek-stop.hpp"

stop_kont_type::stop_kont_type ()
    : kont_type (nullptr, nullptr)
{
}

void
stop_kont_type::print (std::ostream& out) const
{
    out << "(kstop)";
}

void
stop_kont_type::dump (std::ostream& out) const
{
    out << "(kstop)";
}

// C E (kstop) -> C E (kstop)
void
stop_kont_type::eval_step (engine_type* vm, value_type* v)
{
}
