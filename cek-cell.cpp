#include "cek-eval.hpp"

//%markdown
//
// Base Class Cell
// ====
//
// To take flavor of the Baker's treadmill garbage collection (GC), 
// all of objects are doubly-linked together in a ring structure
// keeping the condition:
//
//         cell == cell->gcprev->gcnext
//      && cell == cell->gcnext->gcprev

// make single cell ring structure.
cell_type::cell_type ()
{
    m_gccolor = 0;
    m_gcprev = this;
    m_gcnext = this;
}

// show print style
void
cell_type::print (std::ostream& out) const
{
    out << "<cell>";
}

// show dump style
void
cell_type::dump (std::ostream& out) const
{
    out << "(cell)";
}

// It is necessary to implement for all extended classes
// to copy children into to-space at GC time.
// formal parameter scan is scan pointer of copying GC.
// formal parameter black is current black color code.
//
//     for (cell_type* slot : children) {
//         scan = gccopy (slot, black);
//     }
//     return scan;
//
// only return scan pointer for nothing children.
cell_type*
cell_type::gcscan (cell_type* scan, int black)
{
    return scan;
}

// make cell using by painting black and copying to-space.
// this is core part of stop-and-copying GC.
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

// remove this cell from the treadmill.
cell_type*
cell_type::gcdelete (void)
{
    cell_type* next_cell = m_gcnext;
    m_gcnext->m_gcprev = m_gcprev;
    m_gcprev->m_gcnext = m_gcnext;
    return next_cell;
}

// remove cell and insert cell after *this* cell.
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

// apply style template
//
//     printformat (out, "(fun $1x -> $2m)", m_id, m_exp);
//
// template format:
//
//     '$' [1-5] [dbxomp]
//         ----- --------
//         arg   look
//
// look code:
//
//     'd'  decimal number
//     'b'  boolean notation ("true" / "false")
//     'x'  variable symbol name
//     'o'  binary operator name ("<" / "+" / "*")
//     'm'  recursive print
//     'p'  pinter notation as "%p" of C's sprintf (3) function
//
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
