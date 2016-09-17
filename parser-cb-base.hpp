#pragma once
#include <string>

class parser_callback_base {
public:
    parser_callback_base () {}
    virtual ~parser_callback_base () {}
    virtual void clear (void) = 0;
    virtual void shift (int, std::string const&) = 0;
    virtual void prog_expr (void) = 0;
    virtual void prog_let (void) = 0;
    virtual void prog_let_rec (void) = 0;
    virtual void expr_let (void) = 0;
    virtual void expr_let_rec (void) = 0;
    virtual void expr_if (void) = 0;
    virtual void expr_fun (void) = 0;
    virtual void expr_pexpr (void) = 0;
    virtual void expr_pexpr_lt_pexpr (void) = 0;
    virtual void pexpr_pexpr_plus_mexpr (void) = 0;
    virtual void pexpr_mexpr (void) = 0;
    virtual void mexpr_mexpr_times_app (void) = 0;
    virtual void mexpr_app (void) = 0;
    virtual void app_app_aexpr (void) = 0;
    virtual void app_aexpr (void) = 0;
    virtual void aexpr_id (void) = 0;
    virtual void aexpr_intv (void) = 0;
    virtual void aexpr_bool (void) = 0;
    virtual void aexpr_lparen_expr_rparen (void) = 0;
};
