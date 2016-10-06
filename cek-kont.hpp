#pragma once

// part of cek-eval.hpp
class kont_type : public cell_type {
public:
    explicit kont_type (env_type* e, kont_type* k)
        : cell_type (), m_env (e), m_kont (k) {}
    virtual void eval_step (engine_type* vm, value_type* v) = 0;
    env_type* env (void) const { return m_env; }
    kont_type* kont (void) const { return m_kont; }
    virtual bool isstop (void) const { return false; }
protected:
    env_type* m_env;
    kont_type* m_kont;
};
