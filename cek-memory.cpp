#include "cek.hpp"

int
cek_type::cons (int const tag, int const e1, int const e2, int const e3, int const e4)
{
    gclazy_sweep ();
    int const cell_size = m_cell.size ();
    if (m_sweep >= cell_size) {
        gc (tag, e1, e2, e3, e4);
        if (m_sweep >= cell_size) {
            m_cell.resize (cell_size * 2);
        }
    }
    int const cur = m_sweep++;
    m_cell[cur].tag = tag;
    m_cell[cur].e1 = e1;
    m_cell[cur].e2 = e2;
    m_cell[cur].e3 = e3;
    m_cell[cur].e4 = e4;
    return cur;
}

void
cek_type::gc (int const tag, int const e1, int const e2, int const e3, int const e4)
{
    gcmark (0);
    gcmark (m_ctrl);
    gcmark (m_env);
    gcmark (m_kont);
    gcmark (m_protect);
    if (KIf == tag) {
        gcmark (e1);
    }
    gcmark (e2);
    gcmark (e3);
    gcmark (e4);
    m_sweep = 0;
    gclazy_sweep ();
}

// see D. Knuth ``The Art Of Computer Programming Vol. 1''
//   ISBN 0-201-89683-4 (1997), page 417
//   2.3.5 LISTS AND GARBAGE COLLECTION, Algorithm D (Marking)
void
cek_type::gcmark (int const root)
{
    int const cell_size = m_cell.size ();
    int top = cell_size;
    m_cell[root].forw = top;
    top = root;
    while (top < cell_size) {
        int const cur = top;
        top = m_cell[cur].forw;
        int const child1 = m_cell[cur].e1;
        if (KIf == m_cell[cur].tag && m_cell[child1].forw == 0) {
            m_cell[child1].forw = top;
            top = child1;
        }
        int const child2 = m_cell[cur].e2;
        if (m_cell[child2].forw == 0) {
            m_cell[child2].forw = top;
            top = child2;
        }
        int const child3 = m_cell[cur].e3;
        if (m_cell[child3].forw == 0) {
            m_cell[child3].forw = top;
            top = child3;
        }
        int const child4 = m_cell[cur].e4;
        if (m_cell[child4].forw == 0) {
            m_cell[child4].forw = top;
            top = child4;
        }
    }
}

// see R. John, M. Hughes ``A semi-incremental garbage collection algorithm'',
// Software Practice and Experience, 12 (11) page. 1081, November 1982
void
cek_type::gclazy_sweep (void)
{
    int const cell_size = m_cell.size ();
    while (m_sweep < cell_size && m_cell[m_sweep].forw != 0) {
        m_cell[m_sweep].forw = 0;
        ++m_sweep;
    }
}
