#include "cek-eval.hpp"
#include "cek-blit.hpp"
#include "cek-stop.hpp"
#include "cek-rt-kont.hpp"

engine_type::engine_type ()
{
    m_trace = false;
    m_ctrl = nullptr;
    m_env = nullptr;
    m_kont = nullptr;
    m_protect_kont = nullptr;
    m_shunting_yard = nullptr;
    m_bottom = new cell_type;
    m_top = new cell_type;
    m_bottom->gcinsert (m_top);
    m_scan = m_top;
    m_free = m_top->gcnext ();
    m_black = 1;
    m_gccount = 0;

    m_ctrl = new blit_type (true);
    retain (m_ctrl);
    m_env = new env_type (0, nullptr, nullptr);
    retain (m_env);
    m_kont = new stop_kont_type ();
    retain (m_kont);
}

engine_type::~engine_type ()
{
    cell_type* p = m_bottom;
    do {
        cell_type* cell = p;
        p = p->gcnext ();
        delete cell;        
    } while (p != m_bottom);
}

void
engine_type::eval (std::ostream& out)
{
    // M E0 (krt x E0 (kstop)) -*-> (ref x) (env x v E0) (kstop)
    m_env = m_env->env ();
    m_kont = new rt_kont_type (intern ("it"), m_env, m_kont);
    retain (m_kont);
    m_protect_kont = m_kont;
    print_trace (out);
    do {
        m_ctrl->eval_step (this);
        print_trace (out);
    } while (! m_kont->isstop ());
}

void
engine_type::print_result (std::ostream& out) const
{
    // (ref x) (env x v E0) (kstop)
    if (m_env != nullptr) {
        out << "val " << symbol_name (m_env->id ()) << " = ";
        m_env->value ()->print (out);
    }
    else {
        out << "unexpected CEK state\n";
        print_cek (out);
    }
}

void
engine_type::retain (cell_type* cell)
{
    if (m_free == m_bottom && m_bottom->gcprev () != m_top && m_gccount > 255) {
        gc (cell);
    }
    cell_type* back = m_free->gcprev ();
    ++m_gccount;
    if (m_free != m_bottom) {
        --m_gccount;
        cell_type* wiped = m_free;
        m_free = m_free->gcnext ();
        wiped->gcdelete ();
        delete wiped;
    }
    back->gcinsert (cell);
    cell->gcpaint (m_black);
}

// Stop and Copying Backer's Treadmill
//
//     allocation
//                            >>f                       s
//     [*] [*] [*] [*] [*] [*] [ ] [ ] [ ] [ ] [ ] [B] [T]  -next->
//          painted black         painted white             <-prev-
//     --------------- to-space ------------------
//
//     start gcflip ()  swap B, f and T, s
//      f                                               s
//     [B] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [T]
//         ---------------- from-space ---------------
//
//     end copying
//      f                       s<<
//     [B] [ ] [ ] [ ] [ ] [ ] [T] [*] [*] [*] [*] [*] [*]
//         ---- from space ---     ------- to space ------
//
//     end gcflip ()
//      f                       s
//     [ ] [ ] [ ] [ ] [ ] [B] [T] [*] [*] [*] [*] [*] [*]
//     ---- to space -----         ------- to space ------

void
engine_type::gc (cell_type* cell)
{
    std::swap (m_top, m_bottom);
    std::swap (m_free, m_scan);
    m_black = 1 - m_black;

    m_scan = m_scan->gccopy (m_ctrl, m_black);
    m_scan = m_scan->gccopy (m_env,  m_black);
    m_scan = m_scan->gccopy (m_kont, m_black);
    m_scan = m_scan->gccopy (m_protect_kont, m_black);
    m_scan = m_scan->gccopy (m_shunting_yard, m_black);
    if (cell != nullptr) {
        m_scan = cell->gcscan (m_scan, m_black);
    }
    while (m_scan != m_top) {
        m_scan = m_scan->gcscan (m_scan->gcprev (), m_black);
    }
    if (m_bottom->gcnext () != m_top) {
        m_free = m_bottom->gcnext ();
        m_top->gcprev ()->gcinsert (m_bottom);
    }
}

void
engine_type::print_trace (std::ostream& out) const
{
    if (m_trace) {
        //dump (out);
        print_cek (out);
        out << std::endl;
    }
}

static inline
void
print_reg (std::ostream& out, char const* name, cell_type* reg)
{
    out << " " << name << ": ";
    if (reg == nullptr) {
        out << "()";
    }
    else {
        reg->print (out);
    }
}

void
engine_type::print_cek (std::ostream& out) const
{
    out << "---" << std::endl;
    print_reg (out, "C", m_ctrl); out << std::endl;
    print_reg (out, "E", m_env);  out << std::endl;
    print_reg (out, "K", m_kont);
}

void
engine_type::dump (std::ostream& out) const
{
    out << "---" << std::endl;
    out << "C: " << m_ctrl << std::endl;
    out << "E: " << m_env << std::endl;
    out << "K: " << m_kont << std::endl;
    cell_type* cell = m_bottom;
    do {
        out << (m_bottom == cell ? "B" : m_top == cell ? "T" : " ");
        out << (m_free   == cell ? "f" : " ");
        out << (m_scan   == cell ? "s" : " ");
        out << " " << cell << " ";
        cell->dump (out);
        out << std::endl;
        cell = cell->gcnext ();
    } while (cell != m_bottom);
}
