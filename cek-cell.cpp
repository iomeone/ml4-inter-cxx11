#include "cek-eval.hpp"

cell_type::cell_type ()
{
    m_gccolor = 0;
    m_gcprev = this;
    m_gcnext = this;
}

void
cell_type::print (std::ostream& out) const
{
    out << "<cell>";
}

void
cell_type::dump (std::ostream& out) const
{
    out << "(cell)";
}

cell_type*
cell_type::gcscan (cell_type* scan, int black)
{
    return scan;
}

cell_type*
cell_type::gccopy (cell_type* slot, int black)
{
    cell_type* scan = this;
    if (slot != nullptr && slot->m_gccolor != black) {
        slot->m_gccolor = black;
        scan = scan->gcinsert (slot);
    }
    return scan;
}

cell_type*
cell_type::gcdelete (void)
{
    cell_type* next_cell = m_gcnext;
    m_gcnext->m_gcprev = m_gcprev;
    m_gcprev->m_gcnext = m_gcnext;
    return next_cell;
}

cell_type*
cell_type::gcinsert (cell_type* cell)
{
    cell->m_gcnext->m_gcprev = cell->m_gcprev;
    cell->m_gcprev->m_gcnext = cell->m_gcnext;
    cell->m_gcprev = this;
    cell->m_gcnext = this->m_gcnext;
    cell->m_gcnext->m_gcprev = cell;
    cell->m_gcprev->m_gcnext = cell;
    return cell;
}

void
cell_type::printformat (std::ostream& out, std::string const& src,
    int x1, cell_type const* m2, cell_type const* m3,
            cell_type const* m4, cell_type const* m5) const
{
    for (std::size_t i = 0; i < src.size (); ++i) {
        if ('$' != src[i]) {
            out.put (src[i]);
            continue;
        }
        int const slot = src[++i];
        char const look = src[++i];
        int const x = '1' == slot ? x1 : 0;
        cell_type const* const m = '2' == slot ? m2 : '3' == slot ? m3
            : '4' == slot ? m4 : '5' == slot ? m5 : nullptr;
        switch (look) {
        case 'd':
            out << x;
            break;
        case 'b':
            out << (x ? "true" : "false");
            break;
        case 'x':
            out << symbol_name (x);
            break;
        case 'o':
            switch (x) {
            case PRIM_LT:   out << "<"; break;
            case PRIM_PLUS: out << "+"; break;
            case PRIM_MULT: out << "*"; break;
            default: out << "?"; break;
            }
            break;
        case 'm':
            if (m == nullptr) {
                out << "()";
            }
            else {
                m->print (out);
            }
            break;
        case 'p':
            out << m;
            break;
        }
    }
}
