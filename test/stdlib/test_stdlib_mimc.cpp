#include <gtest/gtest.h>

#include <barretenberg/waffle/composer/standard_composer.hpp>
#include <barretenberg/waffle/composer/mimc_composer.hpp>
#include <barretenberg/waffle/proof_system/preprocess.hpp>
#include <barretenberg/waffle/proof_system/prover/prover.hpp>
#include <barretenberg/waffle/proof_system/verifier/verifier.hpp>

#include <barretenberg/waffle/stdlib/field/field.hpp>
#include <barretenberg/waffle/stdlib/mimc.hpp>

#include <memory>


using namespace barretenberg;
using namespace plonk;

TEST(stdlib_mimc, composer_consistency_check)
{
    waffle::StandardComposer standard_composer = waffle::StandardComposer();
    waffle::MiMCComposer mimc_composer = waffle::MiMCComposer();

    fr::field_t input = fr::random_element();
    fr::field_t k_in = fr::zero;

    stdlib::field_t<waffle::StandardComposer> standard_input(stdlib::witness_t<waffle::StandardComposer>(&standard_composer, input));
    stdlib::field_t<waffle::StandardComposer> standard_k(stdlib::witness_t<waffle::StandardComposer>(&standard_composer, k_in));

    stdlib::field_t<waffle::MiMCComposer> mimc_input(stdlib::witness_t<waffle::MiMCComposer>(&mimc_composer, input));
    stdlib::field_t<waffle::MiMCComposer> mimc_k(stdlib::witness_t<waffle::MiMCComposer>(&mimc_composer, k_in));

    stdlib::field_t<waffle::StandardComposer> standard_out = mimc_block_cipher(standard_input, standard_k);
    standard_out = standard_out.normalize();

    stdlib::field_t<waffle::MiMCComposer> mimc_out = mimc_block_cipher(mimc_input, mimc_k);

    EXPECT_EQ(fr::eq(standard_out.get_value(), mimc_out.get_value()), true);

    waffle::Prover provers[2]{
        standard_composer.preprocess(),
        mimc_composer.preprocess()
    };
    waffle::Verifier verifiers[2]{
         waffle::preprocess(provers[0]),
         waffle::preprocess(provers[1])
    };
    waffle::plonk_proof proofs[2]{
        provers[0].construct_proof(),
        provers[1].construct_proof()
    };
    bool results[2]{
        verifiers[0].verify_proof(proofs[0]),
        verifiers[1].verify_proof(proofs[1])
    };
    EXPECT_EQ(results[0], true);
    EXPECT_EQ(results[1], true);
}

TEST(stdlib_mimc, repeated_hashing)
{
    waffle::MiMCComposer mimc_composer = waffle::MiMCComposer();
    constexpr size_t num_hashes = 100;

    std::vector<stdlib::field_t<waffle::MiMCComposer> > inputs;
    for (size_t i = 0; i < num_hashes; ++i)
    {
        stdlib::field_t<waffle::MiMCComposer> input(stdlib::witness_t<waffle::MiMCComposer>(&mimc_composer, barretenberg::fr::random_element()));
        inputs.push_back(input);
    }

    stdlib::mimc7(inputs);
    waffle::Prover prover = mimc_composer.preprocess();

    waffle::Verifier verifier = waffle::preprocess(prover);

    waffle::plonk_proof proof = prover.construct_proof();

    bool result = verifier.verify_proof(proof);
    EXPECT_EQ(result, true);
}
