#include <limits>
#include <iostream>
#include "symbol.hpp"
#include "cek.hpp"

// CEK-Machine for tiny ML language interpreter

// Matthias Felleisen and Daniel P. Friedman
// ``Control Operators, the SECD-Machine, and the Lambda-Calculus'', (Jun 1986)
//     https://www.cs.indiana.edu/cgi-bin/techreports/TRNNN.cgi?trnum=TR197

cek_type::cek_type ()
{
    m_cell.resize (256);
    m_cell[0].tag = Nil;
    m_sweep = 1;
    m_ctrl = 0;
    m_env = 0;
    m_kont = 0;
    m_protect = 0;
    m_trace = false;
}

cell_type const&
cek_type::at (int addr) const
{
    int const cell_size = m_cell.size ();
    if (addr < 0 || cell_size <= addr) {
        addr = 0;
    }
    return m_cell[addr];
}

void
cek_type::eval (void)
{
    m_kont = cons (KRt, 0, 0, m_env, 0);
    m_protect = m_kont;
    print_trace ();
    do {
        turn ();
        print_trace ();
    } while (m_cell[m_kont].tag != KStop);
}

void
cek_type::print_trace (void)
{
    if (m_trace) {
        std::cout << "---" << std::endl;
        std::cout << "C: " << str (m_ctrl) << std::endl;
        std::cout << "E: " << str (m_env) << std::endl;
        std::cout << "K: " << str (m_kont) << std::endl;
    }
}

void
cek_type::turn_krt (void)
{
    // (Rt v2) E' (KRt _ _ E K) -> (Rt v2) E K
    m_env = m_cell[m_kont].e3;
    m_kont = m_cell[m_kont].e4;
    if (m_kont == 0) {
        m_kont = cons (KStop, 0, 0, 0, 0);
    }
}

void
cek_type::turn_let (int const ktag)
{
    // (Decl    x m1 _ _)  E K -> m1 E (KDecl    x _  E K)
    // (DeclRec x m1 _ _)  E K -> m1 E (KDeclRec x _  E K)
    // (Let     x m1 m2 _) E K -> m1 E (KLet     x m2 E K)
    // (LetRec  x m1 m2 _) E K -> m1 E (KLetRec  x m2 E K)
    int const k1 = cons (ktag, m_cell[m_ctrl].e1, m_cell[m_ctrl].e3, m_env, m_kont);
    m_ctrl = m_cell[m_ctrl].e2;
    m_kont = k1;
}

void
cek_type::turn_klet (int const ktag)
{
    // (Rt _ v _ _) E' (KDecl x _ E K) -> (Rt _ v _ _) (Env x v _ E) K
    // (Rt _ v _ _) E' (KLet  x m E K) -> m            (Env x v _ E) K
    int const e = m_cell[m_kont].e3;
    m_env = cons (Env, m_cell[m_kont].e1, m_cell[m_ctrl].e2, 0, e);
    if (ktag == KLet) {
        m_ctrl = m_cell[m_kont].e2;
    }
    m_kont = m_cell[m_kont].e4;
    if (ktag == KDecl && m_cell[m_kont].tag == KRt) {
        m_cell[m_kont].e3 = m_env;
    }
}

void
cek_type::turn_kletrec (int const ktag)
{
    int const y = m_cell[m_kont].e1;
    int const v = m_cell[m_ctrl].e2;
    int const e = m_cell[m_kont].e3;
    if (m_cell[v].tag != Proc) {
        // (Rt _ v _ _) E' (KLetRec y m E K) -> m (Env y v _ E) K
        m_env = cons (Env, y, v, 0, e);
    }
    else {
        // (Rt _ (Proc x m1 E1 _) _ _) E' (KLetRec y m E K)
        //   -> m (Env y #v=(Proc x m1 (Env y v _ E1) _) _ E) K
        int const e1 = m_cell[v].e3;
        m_cell[v].e3 = cons (Env, y, v, 0, e1);
        if (e1 == e) {
            m_env = m_cell[v].e3;
        }
        else {
            m_env = cons (Env, y, v, 0, e);
        }
    }
    if (ktag == KLetRec) {
        m_ctrl = m_cell[m_kont].e2;
    }
    m_kont = m_cell[m_kont].e4;
    if (ktag == KDeclRec && m_cell[m_kont].tag == KRt) {
        m_cell[m_kont].e3 = m_env;
    }
}

void
cek_type::turn_if (void)
{
    // (If _ m1 m2 m3) E K -> m1 E (KIf _ m2 m3 K)
    int const k1 = cons (KIf, 0, m_cell[m_ctrl].e3, m_cell[m_ctrl].e4, m_kont);
    m_ctrl = m_cell[m_ctrl].e2;
    m_kont = k1;
}

void
cek_type::turn_kif (void)
{
    // (Rt _ (BoolV true  _ _ _) _ _) E (KIf _ m2 m3 K) -> m2 E K
    // (Rt _ (BoolV false _ _ _) _ _) E (KIf _ m2 m3 K) -> m3 E K
    int const v = m_cell[m_ctrl].e2;
    if (m_cell[v].tag != BoolV) {
        croak (ExpectedBoolV);
    }
    else if (m_cell[v].e1) {
        m_ctrl = m_cell[m_kont].e2;
        m_kont = m_cell[m_kont].e4;
    }
    else {
        m_ctrl = m_cell[m_kont].e3;
        m_kont = m_cell[m_kont].e4;
    }
}

void
cek_type::turn_ap (void)
{
    // (Ap _ m1 m2 _) E K -> m1 E (KAp _ m2 E K)
    int const k1 = cons (KAp, 0, m_cell[m_ctrl].e3, m_env, m_kont);
    m_ctrl = m_cell[m_ctrl].e2;
    m_kont = k1;
}

void
cek_type::turn_kap (void)
{
    // (Rt _ (Proc x m1 E1 _) _ _) E' (KAp _ m2 E2 K) -> m2 E2 (KProc x m1 E1 K)
    int const v = m_cell[m_ctrl].e2;
    if (m_cell[v].tag != Proc) {
        croak (ExpectedProc);
    }
    else {
        m_env = m_cell[m_kont].e3;
        int const k = m_cell[m_kont].e4;
        int const k1 = cons (KProc, m_cell[v].e1, m_cell[v].e2, m_cell[v].e3, k);
        m_ctrl = m_cell[m_kont].e2;
        m_kont = k1;
    }
}

void
cek_type::turn_kproc (void)
{
    // (Rt _ v _ _) E' (KProc x m E K) -> m (Env x v _ E) K
    int const v = m_cell[m_ctrl].e2;
    m_env = cons (Env, m_cell[m_kont].e1, v, 0, m_cell[m_kont].e3);
    m_ctrl = m_cell[m_kont].e2;
    m_kont = m_cell[m_kont].e4;
}

void
cek_type::turn_prim (void)
{
    // (Prim op m1 m2 _) E K -> m1 E (KPrim1 op m2 _ K)
    int const k1 = cons (KPrim1, m_cell[m_ctrl].e1, m_cell[m_ctrl].e3, 0, m_kont);
    m_ctrl = m_cell[m_ctrl].e2;
    m_kont = k1;
}

void
cek_type::turn_kprim1 (void)
{
    // (Rt _ v _ _) E (KPrim1 op m _ K) -> m E (KPrim2 op v _ K)
    int const v = m_cell[m_ctrl].e2;
    int const k1 = cons (KPrim2, m_cell[m_kont].e1, v, 0, m_cell[m_kont].e4);
    m_ctrl = m_cell[m_kont].e2;
    m_kont = k1;
}

void
cek_type::turn_kprim2 (void)
{
    // (Rt _ v _ _) E (KPrim2 op u _ K) -> (Rt _ w _ _) E K
    int const op = m_cell[m_kont].e1;
    int const u = m_cell[m_kont].e2;
    int const v = m_cell[m_ctrl].e2;
    if (IntV != m_cell[u].tag || IntV != m_cell[v].tag) {
        croak (ExpectedIntV);
        return;
    }
    if (Lt == op) {
        int const w = cons (BoolV, m_cell[u].e1 < m_cell[v].e1 ? 1 : 0, 0, 0, 0);
        m_ctrl = cons (Rt, 0, w, 0, 0);
    }
    else if (Plus == op) {
        int const w = cons (IntV, m_cell[u].e1 + m_cell[v].e1, 0, 0, 0);
        m_ctrl = cons (Rt, 0, w, 0, 0);
    }
    else if (Mult == op) {
        int const w = cons (IntV, m_cell[u].e1 * m_cell[v].e1, 0, 0, 0);
        m_ctrl = cons (Rt, 0, w, 0, 0);
    }
    m_kont = m_cell[m_kont].e4;
}

void
cek_type::turn_fun (void)
{
    // (Fun x m _ _) E K -> (Rt _ (Proc x m E _) _ _) E K
    int const v = cons (Proc, m_cell[m_ctrl].e1, m_cell[m_ctrl].e2, m_env, 0);
    int const c1 = cons (Rt, 0, v, 0, 0);
    m_ctrl = c1;
}

void
cek_type::turn_ref (void)
{
    // (Ref x _ _ _) E K -> (Rt _ v _ _) E K
    int const e = lookup (m_cell[m_ctrl].e1);
    if (e == 0) {
        croak (UnboundVariable);
    }
    else {
        int const c1 = cons (Rt, 0, m_cell[e].e2, 0, 0);
        m_ctrl = c1;
    }
}

int
cek_type::lookup (int const x)
{
    // (Env x v _ E)
    for (int e = m_env; e != 0; e = m_cell[e].e4) {
        if (x == m_cell[e].e1) {
            return e;
        }
    }
    return 0;
}

void
cek_type::turn_ilit (void)
{
    // (ILit c _ _ _) E K -> (Rt _ (IntV c _ _ _) _ _) E K
    int const v = cons (IntV, m_cell[m_ctrl].e1, 0, 0, 0);
    int const c1 = cons (Rt, 0, v, 0, 0);
    m_ctrl = c1;
}

void
cek_type::turn_blit (void)
{
    // (BLit c _ _ _) E K -> (Rt _ (BoolV c _ _ _) _ _) E K
    int const v = cons (BoolV, m_cell[m_ctrl].e1, 0, 0, 0);
    int const c1 = cons (Rt, 0, v, 0, 0);
    m_ctrl = c1;
}

void
cek_type::croak (int err)
{
    // C E K -> (Error err _ _ _) E protect
    int const c1 = cons (Error, err, 0, 0, 0);
    m_ctrl = c1;
    m_kont = m_protect;
}

void
cek_type::turn_rt (void)
{
    int const ktag = m_kont == 0 ? KStop : m_cell[m_kont].tag;
    switch (ktag) {
    case KStop:
        if (m_kont == 0) {
            m_kont = cons (KStop, 0, 0, 0, 0);
        }
        break;
    case KRt:
        turn_krt ();
        break;
    case KDecl:
    case KLet:
        turn_klet (ktag);
        break;
    case KDeclRec:
    case KLetRec:
        turn_kletrec (ktag);
        break;
    case KIf:
        turn_kif ();
        break;
    case KAp:
        turn_kap ();
        break;
    case KProc:
        turn_kproc ();
        break;
    case KPrim1:
        turn_kprim1 ();
        break;
    case KPrim2:
        turn_kprim2 ();
        break;
    default:
        croak (InvalidContinuation);
        break;
    }
}

void
cek_type::turn (void)
{
    switch (m_ctrl == 0 ? Error : m_cell[m_ctrl].tag) {
    case Decl:
        turn_let (KDecl);
        break;
    case DeclRec:
        turn_let (KDeclRec);
        break;
    case Let:
        turn_let (KLet);
        break;
    case LetRec:
        turn_let (KLetRec);
        break;
    case If:
        turn_if ();
        break;
    case Ap:
        turn_ap ();
        break;
    case Prim:
        turn_prim ();
        break;
    case Fun:
        turn_fun ();
        break;
    case Ref:
        turn_ref ();
        break;
    case ILit:
        turn_ilit ();
        break;
    case BLit:
        turn_blit ();
        break;
    case Rt:
    case Error:
        turn_rt ();
        break;
    default:
        croak (InvalidControl);
        break;
    }
}

std::string
cek_type::result (void) const
{
    if (m_cell[m_ctrl].tag == Rt) {
        return str (m_cell[m_ctrl].e2);
    }
    else if (m_cell[m_ctrl].tag == Error) {
        return str (m_ctrl);
    }
    else {
        std::string t = "unexpected CEK state\n";
        t += " C: " + str (m_ctrl) + "\n";
        t += " E: " + str (m_env) + "\n";
        t += " K: " + str (m_ctrl);
        return t;
    }
}

std::string
cek_type::str (int e) const
{
    std::string t;
    switch (m_cell[e].tag) {
    case Error:
        switch (m_cell[e].e1) {
        case InvalidControl: return "InvalidControl!";
        case InvalidContinuation: return "InvalidContinuation!";
        case ExpectedProc: return "ExpectedProc!";
        case ExpectedBoolV: return "ExpectedBoolV!";
        case ExpectedIntV: return "ExpectedIntV!";
        case UnboundVariable: return "UnboundVariable!";
        }
        return "Error";
    case Nil: return "()";
    case Decl:
        t = "(Decl ";
        t += symbol_table::instance ().name (m_cell[e].e1) + " ";
        t += str (m_cell[e].e2) + ")";
        break;
    case DeclRec:
        t = "(DeclRec ";
        t += symbol_table::instance ().name (m_cell[e].e1) + " ";
        t += str (m_cell[e].e2) + ")";
        break;
    case Let:
        t = "(Let ";
        t += symbol_table::instance ().name (m_cell[e].e1) + " ";
        t += str (m_cell[e].e2) + " ";
        t += str (m_cell[e].e3) + ")";
        break;
    case LetRec:
        t = "(LetRec ";
        t += symbol_table::instance ().name (m_cell[e].e1) + " ";
        t += str (m_cell[e].e2) + " ";
        t += str (m_cell[e].e3) + ")";
        break;
    case If:
        t = "(If ";
        t += str (m_cell[e].e2) + " ";
        t += str (m_cell[e].e3) + " ";
        t += str (m_cell[e].e4) + ")";
        break;
    case Ap:
        t = "(Ap ";
        t += str (m_cell[e].e2) + " ";
        t += str (m_cell[e].e3) + ")";
        break;
    case Prim:
        t = "(";
        switch (m_cell[e].e1) {
        case Lt: t += "< "; break;
        case Plus: t += "+ "; break;
        case Mult: t += "* "; break;
        }
        t += str (m_cell[e].e2) + " ";
        t += str (m_cell[e].e3) + ")";
        break;
    case Fun:
        t = "(Fun ";
        t += symbol_table::instance ().name (m_cell[e].e1) + " ";
        t += str (m_cell[e].e2) + ")";
        break;
    case Ref:
        t = "(Ref ";
        t += symbol_table::instance ().name (m_cell[e].e1) + ")";
        break;
    case ILit:
        t = "(ILit ";
        t += std::to_string (m_cell[e].e1) + ")";
        break;
    case BLit:
        t = "(BLit ";
        t += m_cell[e].e1 ? "true" : "false";
        t += ")";
        break;
    case Rt:
        t = "(Rt ";
        t += str (m_cell[e].e2) + ")";
        break;
    case Proc:
        t = "(Proc ";
        t += symbol_table::instance ().name (m_cell[e].e1) + " ";
        t += "#" + std::to_string (m_cell[e].e2) + "# ";
        t += "#" + std::to_string (m_cell[e].e3) + "#)";
        break;
    case IntV:
        t = std::to_string (m_cell[e].e1);
        break;
    case BoolV:
        t = m_cell[e].e1 ? "true" : "false";
        break;
    case Env:
        t = "(Env ";
        t += symbol_table::instance ().name (m_cell[e].e1) + " ";
        t += str (m_cell[e].e2) + " ";
        t += str (m_cell[e].e4) + ")";
        break;
    case KStop:
        return "(KStop)";
    case KRt:
        t = "(KRt ";
        t += "#" + std::to_string (m_cell[e].e3) + "# ";
        t += str (m_cell[e].e4) + ")";
        break;        
    case KDecl:
        t = "(KDecl ";
        t += symbol_table::instance ().name (m_cell[e].e1) + " ";
        t += "#" + std::to_string (m_cell[e].e3) + "# ";
        t += str (m_cell[e].e4) + ")";
        break;
    case KDeclRec:
        t = "(KDeclRec ";
        t += symbol_table::instance ().name (m_cell[e].e1) + " ";
        t += "#" + std::to_string (m_cell[e].e3) + "# ";
        t += str (m_cell[e].e4) + ")";
        break;
    case KLet:
        t = "(KLet ";
        t += symbol_table::instance ().name (m_cell[e].e1) + " ";
        t += str (m_cell[e].e2) + " ";
        t += "#" + std::to_string (m_cell[e].e3) + "# ";
        t += str (m_cell[e].e4) + ")";
        break;
    case KLetRec:
        t = "(KLetRec ";
        t += symbol_table::instance ().name (m_cell[e].e1) + " ";
        t += str (m_cell[e].e2) + " ";
        t += "#" + std::to_string (m_cell[e].e3) + "# ";
        t += str (m_cell[e].e4) + ")";
        break;
    case KIf:
        t = "(KIf ";
        t += str (m_cell[e].e2) + " ";
        t += str (m_cell[e].e3) + " ";
        t += str (m_cell[e].e4) + ")";
        break;
    case KAp:
        t = "(KAp ";
        t += str (m_cell[e].e2) + " ";
        t += "#" + std::to_string (m_cell[e].e3) + "# ";
        t += str (m_cell[e].e4) + ")";
        break;
    case KProc:
        t = "(KProc ";
        t += symbol_table::instance ().name (m_cell[e].e1) + " ";
        t += str (m_cell[e].e2) + " ";
        t += "#" + std::to_string (m_cell[e].e3) + "# ";
        t += str (m_cell[e].e4) + ")";
        break;
    case KPrim1:
        t = "(KPrim1 ";
        switch (m_cell[e].e1) {
        case Lt: t += "< "; break;
        case Plus: t += "+ "; break;
        case Mult: t += "* "; break;
        }
        t += str (m_cell[e].e2) + " ";
        t += str (m_cell[e].e4) + ")";
        break;
    case KPrim2:
        t = "(KPrim2 ";
        switch (m_cell[e].e1) {
        case Lt: t += "< "; break;
        case Plus: t += "+ "; break;
        case Mult: t += "* "; break;
        }
        t += str (m_cell[e].e2) + " ";
        t += str (m_cell[e].e4) + ")";
        break;
    default:
        return "(Unknown)";
    }
    return t;
}

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

int
cek_type::cons (int const tag, int const e1, int const e2, int const e3, int const e4)
{
    gclazy_sweep ();
    int const cell_size = m_cell.size ();
    if (m_sweep >= cell_size) {
        gc (e2, e3, e4);
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
cek_type::gc (int const e2, int const e3, int const e4)
{
    gcmark (0);
    gcmark (m_ctrl);
    gcmark (m_env);
    gcmark (m_kont);
    gcmark (m_protect);
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
