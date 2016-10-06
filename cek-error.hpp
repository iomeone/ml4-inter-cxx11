#pragma once
#include "cek-eval.hpp"

class error_type : public value_type {
public:
    bool iserror  (void) const;
    void dump (std::ostream& out) const;
};

class unbound_error : public error_type {
public:
    unbound_error (int x);
    int id (void) const { return m_id; }
    void print (std::ostream& out) const;
private:
    int m_id;
};

class expect_bool_error : public error_type {
public:
    void print (std::ostream& out) const;
};

class expect_int_error : public error_type {
public:
    void print (std::ostream& out) const;
};

class expect_closure_error : public error_type {
public:
    void print (std::ostream& out) const;
};

void croak (engine_type* vm, error_type* err);
