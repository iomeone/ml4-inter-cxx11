#pragma once

// part of cek-eval.hpp
class ctrl_type : public cell_type {
public:
    virtual void eval_step (engine_type* vm) = 0;
};
