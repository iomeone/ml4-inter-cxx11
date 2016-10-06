#include "cek-eval.hpp"
#include "cek-error.hpp"
#include "cek-rt-ctrl.hpp"

bool
error_type::iserror  (void) const
{
    return true;
}

void
error_type::dump (std::ostream& out) const
{
    out << "(error \"";
    print (out);
    out << "\")";
}

unbound_error::unbound_error (int x)
    : error_type (), m_id (x)
{
}

void
unbound_error::print (std::ostream& out) const
{
    printformat (out, "unbound variable $1x!", m_id);
}

void 
expect_bool_error::print (std::ostream& out) const
{
    out << "expected boolean!";
}

void 
expect_int_error::print (std::ostream& out) const
{
    out << "expected integer!";
}

void 
expect_closure_error::print (std::ostream& out) const
{
    out << "expected function!";
}

void
croak (engine_type* vm, error_type* err)
{
    vm->m_ctrl = retain_cell (vm, new rt_type (err));
    vm->m_kont = vm->m_protect_kont;
}
