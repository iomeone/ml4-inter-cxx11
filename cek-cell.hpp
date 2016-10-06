#pragma once

// part of cek-eval.hpp
class cell_type {
public:
    cell_type ();
    virtual ~cell_type () {}
    virtual void print (std::ostream& out) const;
    virtual void dump (std::ostream& out) const;
    cell_type* gccopy (cell_type* slot, int black);
    int gccolor (void) const { return m_gccolor; }
    cell_type* gcprev (void) const { return m_gcprev; }
    cell_type* gcnext (void) const { return m_gcnext; }
protected:
    virtual cell_type* gcscan (cell_type* scan, int black);
    void printformat (std::ostream& out, std::string const& src,
        int x1, cell_type const* m2 = nullptr, cell_type const* m3 = nullptr,
                cell_type const* m4 = nullptr, cell_type const* m5 = nullptr) const;
private:
    friend class engine_type;
    void gcpaint (int color) { m_gccolor = color; }
    cell_type* gcdelete (void);
    cell_type* gcinsert (cell_type* cell);
    int m_gccolor;
    cell_type* m_gcprev;
    cell_type* m_gcnext;
};
