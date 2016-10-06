#pragma once

// part of cek-eval.hpp
class engine_type {
public:
    engine_type ();
    ~engine_type ();
    void set_trace (bool b) { m_trace = b; }
    void eval (std::ostream& out);
    void print_result (std::ostream& out) const;
    void print_trace (std::ostream& out) const;
    void print_cek (std::ostream& out) const;
    void retain (cell_type* cell);
    void gc (cell_type* cell);
    void dump (std::ostream& out) const;

    bool m_trace;
    ctrl_type* m_ctrl;
    env_type*  m_env;
    kont_type* m_kont;
    kont_type* m_protect_kont;
    shunting_yard_type* m_shunting_yard;

private:
    cell_type* m_bottom;
    cell_type* m_top;
    cell_type* m_scan;
    cell_type* m_free;
    int m_black;
    int m_gccount;
};
