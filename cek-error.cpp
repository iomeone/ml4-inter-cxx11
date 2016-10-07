#include "cek-eval.hpp"
#include "cek-error.hpp"
#include "cek-rt-ctrl.hpp"

// error variant selector for value of evaluating result

bool
error_type::iserror  (void) const
{
    return true;
}

// show dump style
void
error_type::dump (std::ostream& out) const
{
    out << "(error \"";
    print (out);
    out << "\")";
}

// unbound variable error used at variable reference `cek-ref.cpp`

unbound_error::unbound_error (int x)
    : error_type (), m_id (x)
{
}

void
unbound_error::print (std::ostream& out) const
{
    printformat (out, "unbound variable $1x!", m_id);
}

// expected boolean error used at if-term evaluation `cek-if.cpp`

void 
expect_bool_error::print (std::ostream& out) const
{
    out << "expected boolean!";
}

// expected integer error used at primitive-term evaluation `cek-prim.cpp`

void 
expect_int_error::print (std::ostream& out) const
{
    out << "expected integer!";
}

// expected closure error used at application-term evaluation `cek-app.cpp`

void 
expect_closure_error::print (std::ostream& out) const
{
    out << "expected function!";
}

// handy raising error procedure
// on error, continuation roll back to the initial one.

void
croak (engine_type* vm, error_type* err)
{
    vm->m_ctrl = retain_cell (vm, new rt_type (err));
    vm->m_kont = vm->m_protect_kont;
}
