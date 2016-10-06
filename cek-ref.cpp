#include "cek-eval.hpp"
#include "cek-ref.hpp"
#include "cek-rt-ctrl.hpp"
#include "cek-error.hpp"

ref_type::ref_type (symbol x)
    : term_type (), m_id (x)
{
}

void
ref_type::print (std::ostream& out) const
{
    printformat (out, "$1x", m_id);
}

void
ref_type::dump (std::ostream& out) const
{
    printformat (out, "(ref $1x)", m_id);
}

// (ref x) E K -> (rt v) E K
// (ref x) E K -> (rt unbound_error) E protect
void
ref_type::eval_step (engine_type* vm)
{
    env_type* e = vm->m_env->lookup (m_id);
    if (e != nullptr) {
        vm->m_ctrl = retain_cell (vm, new rt_type (e->value ()));
    }
    else {
        croak (vm, retain_cell (vm, new unbound_error (m_id)));
    }
}
