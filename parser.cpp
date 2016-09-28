#include <deque>
#include "parser.hpp"

// LR Automaton without error recovery.
//
//      state == (RULE[BASE[state] + c] & 0xff)
//      and (ctrl == RULE[BASE[state] + c] >> 8
//        and (ctrl == 0xff then accept
//          or ctrl < 0x80 then shift ctrl
//          or ctrl >= 0x80 then goto 0xfe - ctrl))

// prod ctrl  production
// ----------------------------------------------------------------
//    0   fe prog: expr ";;"
//    1   fd prog: "let" "id" "=" expr ";;"
//    2   fc prog: "let" "rec" "id" "=" funexpr ";;"
//    3   fb expr: "let" "id" "=" expr "in" expr
//    4   fa expr: "let" "rec" "id" "=" funexpr "in" expr
//    5   f9 expr: "if" expr "then" expr "else" expr
//    6   f8 expr: funexpr
//    7   f7 expr: pexpr
//    8   f6 expr: pexpr "<" pexpr
//    9   f5 funexpr: "fun" "id" "->" expr
//   10   f4 pexpr: pexpr "+" mexpr
//   11   f3 pexpr: mexpr
//   12   f2 mexpr: mexpr "*" app
//   13   f1 mexpr: app
//   14   f0 app: app aexpr
//   15   ef app: aexpr
//   16   ee aexpr: "id"
//   17   ed aexpr: "int"
//   18   ec aexpr: "bool"
//   19   eb aexpr: "(" expr ")"

typedef void (*reduce_handler_type)(parser_callback_base& cb);

static void
reduce_prog_expr (parser_callback_base& cb)
{
    cb.prog_expr ();
}

static void
reduce_prog_let (parser_callback_base& cb)
{
    cb.prog_let ();
}

static void
reduce_prog_let_rec (parser_callback_base& cb)
{
    cb.prog_let_rec ();
}

static void
reduce_expr_let (parser_callback_base& cb)
{
    cb.expr_let ();
}

static void
reduce_expr_let_rec (parser_callback_base& cb)
{
    cb.expr_let_rec ();
}

static void
reduce_expr_if (parser_callback_base& cb)
{
    cb.expr_if ();
}

static void
reduce_expr_funexpr (parser_callback_base& cb)
{
    cb.expr_funexpr ();
}

static void
reduce_expr_pexpr (parser_callback_base& cb)
{
    cb.expr_pexpr ();
}

static void
reduce_expr_pexpr_lt_pexpr (parser_callback_base& cb)
{
    cb.expr_pexpr_lt_pexpr ();
}

static void
reduce_funexpr (parser_callback_base& cb)
{
    cb.funexpr ();
}

static void
reduce_pexpr_pexpr_plus_mexpr (parser_callback_base& cb)
{
    cb.pexpr_pexpr_plus_mexpr ();
}

static void
reduce_pexpr_mexpr (parser_callback_base& cb)
{
    cb.pexpr_mexpr ();
}

static void
reduce_mexpr_mexpr_times_app (parser_callback_base& cb)
{
    cb.mexpr_mexpr_times_app ();
}

static void
reduce_mexpr_app (parser_callback_base& cb)
{
    cb.mexpr_app ();
}

static void
reduce_app_app_aexpr (parser_callback_base& cb)
{
    cb.app_app_aexpr ();
}

static void
reduce_app_aexpr (parser_callback_base& cb)
{
    cb.app_aexpr ();
}

static void
reduce_aexpr_id (parser_callback_base& cb)
{
    cb.aexpr_id ();
}

static void
reduce_aexpr_intv (parser_callback_base& cb)
{
    cb.aexpr_intv ();
}

static void
reduce_aexpr_bool (parser_callback_base& cb)
{
    cb.aexpr_bool ();
}

static void
reduce_aexpr_lparen_expr_rparen (parser_callback_base& cb)
{
    cb.aexpr_lparen_expr_rparen ();
}

static reduce_handler_type reduce_handler_table[] = {
    reduce_prog_expr,
    reduce_prog_let,
    reduce_prog_let_rec,
    reduce_expr_let,
    reduce_expr_let_rec,
    reduce_expr_if,
    reduce_expr_funexpr,
    reduce_expr_pexpr,
    reduce_expr_pexpr_lt_pexpr,
    reduce_funexpr,
    reduce_pexpr_pexpr_plus_mexpr,
    reduce_pexpr_mexpr,
    reduce_mexpr_mexpr_times_app,
    reduce_mexpr_app,
    reduce_app_app_aexpr,
    reduce_app_aexpr,
    reduce_aexpr_id,
    reduce_aexpr_intv,
    reduce_aexpr_bool,
    reduce_aexpr_lparen_expr_rparen,
};

static void
reduce_call_handler (int prod, parser_callback_base& cb)
{
    reduce_handler_type handler = reduce_handler_table[prod];
    handler (cb);
}

static const unsigned short PRODUCTION[] = {
    0x1902, 0x1905, 0x1906, 0x1806, 0x1807, 0x1806, 0x1801, 0x1801,
    0x1803, 0x1604, 0x1703, 0x1701, 0x1503, 0x1501, 0x1402, 0x1401,
    0x1301, 0x1301, 0x1301, 0x1303,
};
static const short BASE[] = {
    0, -12, 14, 14, 5, 9, 33, 38, 2, 45, 53, 72, 90, 108, 126, 133, 18, 10,
    47, 60, 70, 143, 152, 54, 84, 173, 60, 180, 57, 193, 58, 67, 116, 217,
    214, 238, 257, 82, 254, 82, 265, 126, 135, 165, 278, 181, 291, 182, 302,
    246, 185, 313, 316, 184, 337
};
static const unsigned short RULE[] = {
    0x0401, 0x0501, 0x0801, 0x0c01, 0x0d01, 0x0e01, 0x0f01, 0x0b01,
    0x0a01, 0x0901, 0x0601, 0x0701, 0x0301, 0x0201, 0xff02, 0x1003,
    0x1204, 0x1708, 0xfe10, 0x1b11, 0x1104, 0x1405, 0x0505, 0x0805,
    0x0c05, 0x0d05, 0x0e05, 0x0f05, 0x0b05, 0x0a05, 0x0905, 0x0605,
    0x0705, 0x1305, 0xf806, 0xf806, 0xf806, 0xf806, 0xf806, 0xf707,
    0xf707, 0xf707, 0xf707, 0xf707, 0x1607, 0x1507, 0xf309, 0xf309,
    0xf309, 0xf309, 0xf309, 0xf309, 0xf309, 0x1809, 0xf10a, 0xf10a,
    0xf10a, 0xf10a, 0xf10a, 0xf10a, 0xf10a, 0xf10a, 0x1c12, 0x1d13,
    0x2217, 0x241a, 0x261c, 0x281e, 0x0c0a, 0x0d0a, 0x0e0a, 0x0f0a,
    0x190a, 0xef0b, 0xef0b, 0xef0b, 0xef0b, 0xef0b, 0xef0b, 0xef0b,
    0xef0b, 0x1f14, 0x291f, 0x2b25, 0x2c25, 0x1e14, 0x2e27, 0xef0b,
    0xef0b, 0xef0b, 0xef0b, 0xee0c, 0xee0c, 0xee0c, 0xee0c, 0xee0c,
    0xee0c, 0xee0c, 0xee0c, 0x0c18, 0x0d18, 0x0e18, 0x0f18, 0x0b18,
    0x2318, 0xee0c, 0xee0c, 0xee0c, 0xee0c, 0xed0d, 0xed0d, 0xed0d,
    0xed0d, 0xed0d, 0xed0d, 0xed0d, 0xed0d, 0xf620, 0xf620, 0xf620,
    0xf620, 0xf620, 0x1620, 0xed0d, 0xed0d, 0xed0d, 0xed0d, 0xec0e,
    0xec0e, 0xec0e, 0xec0e, 0xec0e, 0xec0e, 0xec0e, 0xec0e, 0x3029,
    0xf52a, 0xf52a, 0xf52a, 0xf52a, 0xf52a, 0xec0e, 0xec0e, 0xec0e,
    0xec0e, 0x140f, 0x050f, 0x080f, 0x0c0f, 0x0d0f, 0x0e0f, 0x0f0f,
    0x0b0f, 0x0a0f, 0x090f, 0x060f, 0x070f, 0x1a0f, 0x0c15, 0x0d15,
    0x0e15, 0x0f15, 0x0b15, 0x0a15, 0x0915, 0xfd2b, 0x2015, 0x0c16,
    0x0d16, 0x0e16, 0x0f16, 0x0b16, 0x0a16, 0x2116, 0xf019, 0xf019,
    0xf019, 0xf019, 0xf019, 0xf019, 0xf019, 0xf019, 0x322d, 0x332d,
    0x2c2f, 0xfc32, 0x3335,      0, 0xf019, 0xf019, 0xf019, 0xf019,
    0x141b, 0x051b, 0x081b, 0x0c1b, 0x0d1b, 0x0e1b, 0x0f1b, 0x0b1b,
    0x0a1b, 0x091b, 0x061b, 0x071b, 0x251b, 0x141d, 0x051d, 0x081d,
    0x0c1d, 0x0d1d, 0x0e1d, 0x0f1d, 0x0b1d, 0x0a1d, 0x091d, 0x061d,
    0x071d, 0x271d, 0xf421, 0xf421, 0xf421, 0xf421, 0xf421, 0xf421,
    0xf421, 0x1821, 0x1422, 0x0522, 0x0822, 0x0c22, 0x0d22, 0x0e22,
    0x0f22, 0x0b22, 0x0a22, 0x0922, 0x0622, 0x0722, 0x2a22, 0xf223,
    0xf223, 0xf223, 0xf223, 0xf223, 0xf223, 0xf223, 0xf223, 0xfb31,
    0xfb31, 0xfb31, 0xfb31, 0xfb31,      0, 0x0c23, 0x0d23, 0x0e23,
    0x0f23, 0x1923, 0xeb24, 0xeb24, 0xeb24, 0xeb24, 0xeb24, 0xeb24,
    0xeb24, 0xeb24,      0,      0, 0x0826,      0,      0,      0,
    0xeb24, 0xeb24, 0xeb24, 0xeb24, 0x2d26, 0x1428, 0x0528, 0x0828,
    0x0c28, 0x0d28, 0x0e28, 0x0f28, 0x0b28, 0x0a28, 0x0928, 0x0628,
    0x0728, 0x2f28, 0x142c, 0x052c, 0x082c, 0x0c2c, 0x0d2c, 0x0e2c,
    0x0f2c, 0x0b2c, 0x0a2c, 0x092c, 0x062c, 0x072c, 0x312c, 0x142e,
    0x052e, 0x082e, 0x0c2e, 0x0d2e, 0x0e2e, 0x0f2e, 0x0b2e, 0x0a2e,
    0x092e, 0x062e, 0x072e, 0x342e, 0x0830, 0xf934, 0xf934, 0xf934,
    0xf934, 0xf934,      0,      0, 0x3530, 0x1433, 0x0533, 0x0833,
    0x0c33, 0x0d33, 0x0e33, 0x0f33, 0x0b33, 0x0a33, 0x0933, 0x0633,
    0x0733, 0x3633, 0xfa36, 0xfa36, 0xfa36, 0xfa36, 0xfa36,
};

bool
parse (std::istream& cin, scanner_type& token, parser_callback_base& cb)
{
    const int ncheck = sizeof (RULE) / sizeof (RULE[0]);
    if (! token.next (cin)) {
        return false;
    }
    std::deque<int> kont {1};
    for (;;) {
        int const state = kont.back ();
        int const kind = token.kind ();
        int const i = BASE[state] + kind;
        if (i < 0 || ncheck <= i || state != (RULE[i] & 0xff)) {
            break;
        }
        int const ctrl = RULE[i] >> 8;
        if (0xff == ctrl) {
            return true;
        }
        else if (ctrl < 0x80) {
            cb.shift (kind, token.value ());
            kont.push_back (ctrl);
            if (! token.next (cin)) {
                break;
            }
        }
        else {
            int const prod = 0xfe - ctrl;
            reduce_call_handler (prod, cb);
            for (int i = PRODUCTION[prod] & 0xff; i > 0; --i) {
                kont.pop_back ();
            }
            int const state1 = kont.back ();
            int const j = BASE[state1] + (PRODUCTION[prod] >> 8);
            if (j < 0 || ncheck <= j || state1 != (RULE[j] & 0xff)) {
                break;
            }
            kont.push_back (RULE[j] >> 8);
        }
    }
    return false;
}
