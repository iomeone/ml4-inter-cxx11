#include "cek.hpp"

void
cek_type::begin_ctrl (void)
{
    m_ctrl = cons (Env, 0, 0, 0, 0);
}

void
cek_type::ctrl_shift (int lit)
{
    int const v = cons (ILit, lit, 0, 0, 0);
    int const e = cons (Env, 0, v, 0, m_ctrl);
    m_ctrl = e;
}

int
cek_type::ctrl_lit (int nth)
{
    int const exp = ctrl_exp (nth);
    return m_cell[exp].e1;
}

int
cek_type::ctrl_exp (int nth)
{
    int p = m_ctrl;
    for (int i = 1; i < nth; ++i) {
        p = m_cell[p].e4;
    }
    return m_cell[p].e2;
}

void
cek_type::ctrl_reduce (int rhs_len, int exp)
{
    int p = m_ctrl;
    for (int i = 1; i < rhs_len; ++i) {
        p = m_cell[p].e4;
    }
    m_cell[p].e2 = exp;
    m_ctrl = p;
}

void
cek_type::ctrl_reduce (int rhs_len,
    int const tag, int const e1, int const e2, int const e3, int const e4)
{
    int p = m_ctrl;
    for (int i = 1; i < rhs_len; ++i) {
        p = m_cell[p].e4;
    }
    m_cell[p].e2 = cons (tag, e1, e2, e3, e4);
    m_ctrl = p;
}

void
cek_type::end_ctrl (void)
{
    m_ctrl = m_cell[m_ctrl].e2;
}
