#include "symbol.hpp"
#include "cek.hpp"

// CEK-Machine for tiny ML language interpreter

// Matthias Felleisen and Daniel P. Friedman
// ``Control Operators, the SECD-Machine, and the Lambda-Calculus'', (Jun 1986)
//     https://www.cs.indiana.edu/cgi-bin/techreports/TRNNN.cgi?trnum=TR197

enum { IGNORE_DUMMY_VALUE = -1 };

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
    m_kont = cons (KRt, 0, 0, m_env, cons (KStop, 0, 0, 0, 0));
    m_protect = m_kont;
    print_trace ();
    do {
        turn ();
        print_trace ();
    } while (m_cell[m_kont].tag != KStop);
}

void
cek_type::turn (void)
{
    if (0 == m_ctrl) {
        return croak (InvalidControl);
    }
    switch (m_cell[m_ctrl].tag) {
    case Decl:
        turn_let (KDecl);
        break;
    case DeclRec:
        turn_letrec (KDeclRec);
        break;
    case Let:
        turn_let (KLet);
        break;
    case LetRec:
        turn_letrec (KLetRec);
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

void
cek_type::turn_rt (void)
{
    if (0 == m_kont) {
        return croak (InvalidContinuation);
    }
    int const ktag = m_cell[m_kont].tag;
    switch (ktag) {
    case KStop:
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
    // (Let     x m1 m2 _) E K -> m1 E (KLet     x m2 E K)
    m_kont = cons (ktag, m_cell[m_ctrl].e1, m_cell[m_ctrl].e3, m_env, m_kont);
    m_ctrl = m_cell[m_ctrl].e2;
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
cek_type::turn_letrec (int const ktag)
{
    // (DeclRec x m1 _ _)  E K -> m1 E1=(x DUMMY E) (KDeclRec x _  E1 K)
    // (LetRec  x m1 m2 _) E K -> m1 E1=(x DUMMY E) (KLetRec  x m2 E1 K)
    m_env = cons (Env, m_cell[m_ctrl].e1, IGNORE_DUMMY_VALUE, 0, m_env);
    m_kont = cons (ktag, m_cell[m_ctrl].e1, m_cell[m_ctrl].e3, m_env, m_kont);
    m_ctrl = m_cell[m_ctrl].e2;
}

void
cek_type::turn_kletrec (int const ktag)
{
    // (Rt _ v _ _) E' (KDeclRec x _  E1=(x DUMMY E) K) -> (Rt _ v _ _) E1=(x v E) K
    // (Rt _ v _ _) E' (KLetRec  x m2 E1=(x DUMMY E) K) -> m2 E1=(x v E) K
    int const e = m_cell[m_kont].e3;
    m_cell[e].e2 = m_cell[m_ctrl].e2;
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
    // (If _ m1 m2 m3) E K -> m1 E (KIf m2 m3 E K)
    int const k1 = cons (KIf, m_cell[m_ctrl].e3, m_cell[m_ctrl].e4, m_env, m_kont);
    m_ctrl = m_cell[m_ctrl].e2;
    m_kont = k1;
}

void
cek_type::turn_kif (void)
{
    // (Rt _ (BoolV true  _ _ _) _ _) E1 (KIf m2 m3 E K) -> m2 E K
    // (Rt _ (BoolV false _ _ _) _ _) E2 (KIf m2 m3 E K) -> m3 E K
    int const v = m_cell[m_ctrl].e2;
    if (m_cell[v].tag != BoolV) {
        croak (ExpectedBoolV);
    }
    else if (m_cell[v].e1) {
        m_ctrl = m_cell[m_kont].e1;
        m_env  = m_cell[m_kont].e3;
        m_kont = m_cell[m_kont].e4;
    }
    else {
        m_ctrl = m_cell[m_kont].e2;
        m_env  = m_cell[m_kont].e3;
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
    // (Prim op m1 m2 _) E K -> m1 E (KPrim1 op m2 E K)
    int const k1 = cons (KPrim1, m_cell[m_ctrl].e1, m_cell[m_ctrl].e3, m_env, m_kont);
    m_ctrl = m_cell[m_ctrl].e2;
    m_kont = k1;
}

void
cek_type::turn_kprim1 (void)
{
    // (Rt _ v _ _) E1 (KPrim1 op m E K) -> m E (KPrim2 op v E K)
    int const v = m_cell[m_ctrl].e2;
    m_ctrl = m_cell[m_kont].e2;
    m_env  = m_cell[m_kont].e3;
    int const k1 = cons (KPrim2, m_cell[m_kont].e1, v, m_env, m_cell[m_kont].e4);
    m_kont = k1;
}

void
cek_type::turn_kprim2 (void)
{
    // (Rt _ v _ _) E1 (KPrim2 op u E K) -> (Rt _ w _ _) E K
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
    m_env  = m_cell[m_kont].e3;
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
        if (x == m_cell[e].e1 && m_cell[e].e2 != IGNORE_DUMMY_VALUE) {
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
