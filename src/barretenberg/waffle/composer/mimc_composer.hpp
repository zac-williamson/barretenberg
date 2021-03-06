#pragma once

#include "./standard_composer.hpp"

namespace waffle
{
struct mimc_quadruplet
{
    uint32_t x_in_idx;
    uint32_t x_cubed_idx;
    uint32_t k_idx;
    uint32_t x_out_idx;
    barretenberg::fr::field_t mimc_constant;
};

class MiMCComposer : public StandardComposer
{
public:
    MiMCComposer(const size_t size_hint = 0) : StandardComposer()
    {
        q_mimc_coefficient.reserve(size_hint);
        q_mimc_selector.reserve(size_hint);
        add_variable(barretenberg::fr::field_t({{0,0,0,0}}));
        zero_idx = 0;
        features |= static_cast<size_t>(Features::MIMC_SELECTORS);
    };
    MiMCComposer(MiMCComposer &&other) = default;
    MiMCComposer& operator=(MiMCComposer &&other) = default;

    ~MiMCComposer() {}

    Prover preprocess() override;

    void create_add_gate(const add_triple &in) override;
    void create_mul_gate(const mul_triple &in) override;
    void create_bool_gate(const uint32_t a) override;
    void create_poly_gate(const poly_triple &in) override;
    void create_mimc_gate(const mimc_quadruplet &in);
    void create_noop_gate();
    void create_dummy_gates();
    size_t get_num_constant_gates() const override { return StandardComposer::get_num_constant_gates(); }

    std::vector<barretenberg::fr::field_t> q_mimc_coefficient;
    std::vector<barretenberg::fr::field_t> q_mimc_selector;

    uint32_t current_output_wire = static_cast<uint32_t>(-1);
    uint32_t zero_idx;
};
}
