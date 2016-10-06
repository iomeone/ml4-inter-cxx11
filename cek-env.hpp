#pragma once

// part of cek-eval.hpp
class env_type : public cell_type {
public:
    explicit env_type (symbol x, value_type* v, env_type* e);
    void print (std::ostream& out) const;
    void dump (std::ostream& out) const;
    env_type* lookup (symbol x);
    symbol id (void) const { return m_id; }
    value_type* value (void) const { return m_value; }
    env_type* env (void) const { return m_env; }
    void set_value (value_type* v) { m_value = v; }
protected:
    cell_type* gcscan (cell_type* scan, int black);
private:
    symbol m_id;
    value_type* m_value;
    env_type* m_env;
};
