#include "cek-eval.hpp"
#include "cek-blit.hpp"
#include "cek-stop.hpp"
#include "cek-rt-kont.hpp"

//%markdown
//
// Engine of CEK Machine for ML<sup>4</sup> Interpreter
// =====
//
// The engine conducts terms and continuations to evaluate the
// abstract syntax tree (AST) of ML<sup>4</sup> under the environment.
// The term objects and continuation objects themselves carry out
// most operations of evaluations and the object space managements.
// The engine only calls their virtual member functions.
//
// The envirionment is a ordered set of the pair of a symbol and a value.
// Default environment is empty. Some declaration AST will extend the
// environment as user's input.
//
// The evaluation of the given AST from an intial state of three registers:
// the control register C, the environment register E, and the continuation
// register K.
//
//      C is pointed root term of AST
//      E is pointed (env last-variable last-value next-environment)
//      K is initialized internally
//
// Last evaluated result at the top of the environment is droped
// before starting of the evaluation by the engine.
//
// The engine turns each step of CEK states calling virtual member
// function `eval_step` of control/term objects for each times.
// See each control/term sources for details of CEK states transitions.
//
// After the evaluation, the result of value puts at the top of
// the environment with the variable of declaration or the default one
// `it`. `print_result` member function shows the result by the
// ML<sup>4</sup> style output from the environment.
//
// Baker's treadmill provides you to manage the object space.
// In it, all of objects are linked together as a ring structure.
// To keep a pointer to an object in the treadmill, template function
// `retain_cell` works fine for you instead of member function `retain`.
// The engine often invokes stop and copying garbage collection (GC)
// without relocations. It deletes useless objects.

// Makes empty treadmill and sets CEK register mock last result.
engine_type::engine_type ()
{
    m_trace = false;
    // root registers must be unset
    // to avoid strange copying at the first GC time.
    m_ctrl = nullptr;
    m_env = nullptr;
    m_kont = nullptr;
    m_protect_kont = nullptr;
    m_shunting_yard = nullptr;
    // make empty treadmill
    m_bottom = new cell_type;
    m_top = new cell_type;
    m_bottom->gcinsert (m_top);
    m_scan = m_top;
    m_free = m_top->gcnext ();
    m_black = 1;
    m_gccount = 0;
    // setup mock last result: (BLit true) (env 0 () ()) (kstop)
    m_ctrl = new blit_type (true);
    retain (m_ctrl);
    m_env = new env_type (0, nullptr, nullptr);
    retain (m_env);
    m_kont = new stop_kont_type ();
    retain (m_kont);
}

// wipe out object space
engine_type::~engine_type ()
{
    cell_type* p = m_bottom;
    do {
        cell_type* cell = p;
        p = p->gcnext ();
        delete cell;        
    } while (p != m_bottom);
}

// evaluate AST under the environment. 
void
engine_type::eval (std::ostream& out)
{
    // E and K as the last evaluating.
    // M (env x' v' E0) (kstop) -> M E0 (krt x E0 (kstop))
    m_env = m_env->env ();
    m_kont = new rt_kont_type (intern ("it"), m_env, m_kont);
    retain (m_kont);
    m_protect_kont = m_kont;
    // M E0 (krt x E0 (kstop)) -*-> (ref x) (env x v E0) (kstop)
    print_trace (out);
    do {
        m_ctrl->eval_step (this);
        print_trace (out);
    } while (! m_kont->isstop ());
}

// show the evaluation results by the ML<sup>4</sup> style.
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

// bless a pinter to a cell to control under garbage collection. 
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
//     start GC  swap B, f and T, s
//      f                                               s
//     [B] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ] [T]
//         ---------------- from-space ---------------
//
//     end copying
//      f                       s<<
//     [B] [ ] [ ] [ ] [ ] [ ] [T] [*] [*] [*] [*] [*] [*]
//         ---- from space ---     ------- to space ------
//
//     end GC
//      f                       s
//     [ ] [ ] [ ] [ ] [ ] [B] [T] [*] [*] [*] [*] [*] [*]
//     ---- to space -----         ------- to space ------
//
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
        // depth first trace
        m_scan = m_scan->gcscan (m_scan->gcprev (), m_black);
    }
    if (m_bottom->gcnext () != m_top) {
        m_free = m_bottom->gcnext ();
        m_top->gcprev ()->gcinsert (m_bottom);
    }
}

// show CEK state when `set_trace (true);`
void
engine_type::print_trace (std::ostream& out) const
{
    if (m_trace) {
        //dump (out);
        print_cek (out);
        out << std::endl;
    }
}

// show register and its content.
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

// show CEK state.
void
engine_type::print_cek (std::ostream& out) const
{
    out << "---" << std::endl;
    print_reg (out, "C", m_ctrl); out << std::endl;
    print_reg (out, "E", m_env);  out << std::endl;
    print_reg (out, "K", m_kont);
}

// show CEK state and object space by the dump style.
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
