#pragma once

// part of cek-eval.hpp
class value_type : public cell_type {
public:
    virtual bool intv (int& i) const { return false; }
    virtual bool boolv (bool& b) const { return false; }
    virtual bool closure (symbol& x, term_type*& m, env_type*& e) { return false; }
    virtual bool iserror (void) const { return false; }
};
