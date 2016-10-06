#pragma once

// part of cek-eval.hpp
class shunting_yard_type : public cell_type {
public:
    shunting_yard_type ();
    void print (std::ostream& out) const;
    void dump (std::ostream& out) const;
    void shift (int v);
    int lit (int i) const;
    term_type* term (int i) const;
    void reduce (int rhs_len, term_type* term);
protected:
    cell_type* gcscan (cell_type* scan, int black);
private:
    std::vector<std::pair<int, term_type*>> m_data;
};
