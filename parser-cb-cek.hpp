#pragma once
#include "parser-cb-base.hpp"
#include "symbol.hpp"
#include "cek.hpp"

class parser_cb_cek_type : public parser_callback_base {
public:
    parser_cb_cek_type (cek_type* cek);
    void clear (void);
    void shift (int kind, std::string const& value);
    void prog_expr (void);
    void prog_let (void);
    void prog_let_rec (void);
    void expr_let (void);
    void expr_let_rec (void);
    void expr_if (void);
    void expr_fun (void);
    void expr_pexpr (void);
    void expr_pexpr_lt_pexpr (void);
    void pexpr_pexpr_plus_mexpr (void);
    void pexpr_mexpr (void);
    void mexpr_mexpr_times_app (void);
    void mexpr_app (void);
    void app_app_aexpr (void);
    void app_aexpr (void);
    void aexpr_id (void);
    void aexpr_intv (void);
    void aexpr_bool (void);
    void aexpr_lparen_expr_rparen (void);

private:
    cek_type* m_cek;
};
