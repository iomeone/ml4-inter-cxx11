#pragma once

// part of cek-eval.hpp

class engine_type {
public:
    engine_type ();
    ~engine_type ();
    // turn true to print CEK status for each step
    void set_trace (bool b) { m_trace = b; }
    // evaluate term pointed by Control Register under the Environment
    void eval (std::ostream& out);
    // show evaluated result
    void print_result (std::ostream& out) const;
    // show CEK status only when trace flag set
    void print_trace (std::ostream& out) const;
    // show CEK status
    void print_cek (std::ostream& out) const;
    // manage cell pointer with Garbage Collection
    void retain (cell_type* cell);
    // Garbage Collection: stop and copying without relocations
    void gc (cell_type* cell);
    // list all managed cell
    void dump (std::ostream& out) const;

private:
    // Memory management Treadmill's pointer
    cell_type* m_bottom;                    // B pointer
    cell_type* m_top;                       // T pointer
    cell_type* m_scan;                      // scan pointer
    cell_type* m_free;                      // free pointer
    int m_black;                            // black color code 0 or 1
    int m_gccount;                          // count of live cells

    bool m_trace;

    // CEK machine's Register
    ctrl_type* m_ctrl;                      // Control Register
    env_type*  m_env;                       // Environment Register
    kont_type* m_kont;                      // Continuation Register
    kont_type* m_protect_kont;              // error catch continuation
    shunting_yard_type* m_shunting_yard;    // to build term tree
};
