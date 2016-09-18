#include "cek.hpp"

void
cek_type::begin_ctrl (void)
{
    m_ctrl = cons (Env, 0, 0, 0, 0);
}

void
cek_type::ctrl_shift (int lit)
{
    int const e = cons (Env, lit, 0, 0, m_ctrl);
    m_ctrl = e;
}

int
cek_type::ctrl_lit (int nth)
{
    int const p = ctrl_nth_leftvalue (nth);
    return m_cell[p].e1;
}

int
cek_type::ctrl_exp (int nth)
{
    int const p = ctrl_nth_leftvalue (nth);
    return m_cell[p].e2;
}

void
cek_type::ctrl_reduce (int rhs_len, int exp)
{
    int const p = ctrl_nth_leftvalue (rhs_len);
    m_cell[p].e2 = exp;
    m_ctrl = p;
}

void
cek_type::ctrl_reduce (int rhs_len,
    int const tag, int const e1, int const e2, int const e3, int const e4)
{
    ctrl_reduce (rhs_len, cons (tag, e1, e2, e3, e4));
}

void
cek_type::end_ctrl (void)
{
    m_ctrl = m_cell[m_ctrl].e2;
}

int
cek_type::ctrl_nth_leftvalue (int const nth)
{
    int p = m_ctrl;
    for (int i = 1; i < nth; ++i) {
        p = m_cell[p].e4;
    }
    return p;
}
