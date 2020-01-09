#pragma once

#include <memory>

#include "../../../polynomials/polynomial.hpp"
#include "../../../types.hpp"

#include "../../reference_string/reference_string.hpp"
#include "../../waffle_types.hpp"
#include "../widgets/base_widget.hpp"

#include "../circuit_state.hpp"

namespace waffle
{

template<size_t program_width>
class Prover
{
  public:
    Prover(const size_t n = 0);
    Prover(Prover&& other);
    Prover(const Prover& other) = delete;
    Prover(const CircuitFFTState& fftState);
    Prover& operator=(const Prover& other) = delete;
    Prover& operator=(Prover&& other);

    void compute_permutation_lagrange_base_full();
    void compute_wire_coefficients();
    void compute_z_coefficients();
    void compute_wire_commitments();
    void compute_z_commitment();
    void compute_quotient_commitment();
    void compute_permutation_grand_product_coefficients(barretenberg::polynomial& z_fft);
    void compute_identity_grand_product_coefficients(barretenberg::polynomial& z_fft);
    void compute_arithmetisation_coefficients();
    void compute_quotient_polynomial();
    void compute_opening_elements();
    barretenberg::fr::field_t compute_linearisation_coefficients();
    plonk_proof construct_proof();
    void reset();

    size_t n;


    std::array<barretenberg::polynomial, program_width> w;
    std::array<barretenberg::polynomial, program_width> sigma;
    barretenberg::polynomial z;

    barretenberg::polynomial r;

    waffle::CircuitFFTState fft_state;

    std::array<std::vector<uint32_t>, program_width> sigma_map;

    // Hmm, mixing runtime polymorphism and zero-knowledge proof generation. This seems fine...
    std::vector<std::unique_ptr<ProverBaseWidget>> widgets;
    plonk_challenges challenges{};
    plonk_proof proof;
    ReferenceString reference_string;
};

} // namespace waffle

#include "./prover.tcc"