#pragma once
#include <vector>
#include <string>

enum { Lt, Plus, Mult };

enum {
    Success,
    InvalidControl,
    InvalidContinuation,
    ExpectedProc,
    ExpectedBoolV,
    ExpectedIntV,
    UnboundVariable
};

// tag value for cell_type
enum {
    Error,
    Nil,
    // control cell
    Decl,       // id exp1 _ _      `let id = exp;;`
    DeclRec,    // id exp1 _ _      `let rec id = exp;;`
    Let,        // id exp1 exp2 _   `let id = exp1 in exp2`
    LetRec,     // id exp1 exp2 _   `let rec id = exp1 in exp2`
    If,         // _ exp1 exp2 exp3 `if exp1 then exp2 else exp3`
    Ap,         // _ exp1 exp2 _    application `exp1 exp2`
    Prim,       // op exp1 exp2 _   binary operator primitive
    Fun,        // id exp _ _       function declaration
    Ref,        // id _ _ _         variable reference
    ILit,       // int _ _ _        integer literal
    BLit,       // bool _ _ _       boolean literal
    Rt,         // _ val _ _        apply kontinuation value
    // Rt value
    Proc,       // id exp env _     procedure
    IntV,       // int _ _ _        integer value
    BoolV,      // bool _ _ _       boolean value
    // environment cell
    Env,        // id val _ E       environment
    // continuation cell
    KStop,      // _ _ _ _
    KRt,        // _ _ env K        fun x -> x
    KDecl,      // id _ env K       continuation for declaration
    KDeclRec,   // id _ env K       continuation for let rec declaration
    KLet,       // id exp env K     continuation for let expression
    KLetRec,    // id exp env K     continuation for let rec expression
    KIf,        // _ exp2 exp3 K    continuation for if expression
    KAp,        // _ exp env K      continuation for application expression
    KProc,      // id exp env K     continuation for procedure
    KPrim1,     // op exp _ K       first continuation for primitive
    KPrim2,     // op val _ K       second continuation for primitive
};

struct cell_type {
    int forw;
    int tag;
    int e1;
    int e2;
    int e3;
    int e4;

    cell_type ()
    {
        forw = 0;
        tag = Error;
        e1 = 0;
        e2 = 0;
        e3 = 0;
        e4 = 0;
    }

    explicit cell_type (int t, int a, int b, int c, int d)
    {
        forw = 0;
        tag = t;
        e1 = a;
        e2 = b;
        e3 = c;
        e4 = d;
    }
};

class cek_type {
public:
    cek_type ();
    int ctrl (void) const { return m_ctrl; }
    int env (void) const { return m_env; }
    int kont (void) const { return m_kont; }
    bool trace (void) const { return m_trace; }
    void set_trace (bool x) { m_trace = x; }
    std::string str (int e) const;
    std::string result (void) const;
    std::string str_cek (void) const;
    cell_type const& at (int addr) const;
    int lookup (int const x);
    void eval (void);
    void turn (void);
    // parser_callback interface
    void begin_ctrl (void);
    void ctrl_shift (int lit);
    int ctrl_lit (int nth);
    int ctrl_exp (int nth);
    void ctrl_reduce (int rhs_len, int exp);
    void ctrl_reduce (int rhs_len,
        int const tag, int const e1, int const e2, int const e3, int const e4);
    void end_ctrl (void);

private:
    void print_trace (void);
    int cons (int const tag, int const e1, int const e2, int const e3, int const e4);
    void turn_krt (void);
    void turn_let (int const ktag);
    void turn_klet (int const ktag);
    void turn_kletrec (int const ktag);
    void turn_if (void);
    void turn_kif (void);
    void turn_ap (void);
    void turn_kap (void);
    void turn_kproc (void);
    void turn_prim (void);
    void turn_kprim1 (void);
    void turn_kprim2 (void);
    void turn_fun (void);
    void turn_ref (void);
    void turn_ilit (void);
    void turn_blit (void);
    void turn_rt (void);
    void croak (int err);
    void gc (int const e2, int const e3, int const e4);
    void gcmark (int const root);
    void gclazy_sweep (void);

    std::vector<cell_type> m_cell;
    int m_sweep;
    int m_ctrl, m_env, m_kont, m_protect;
    bool m_trace;
};
