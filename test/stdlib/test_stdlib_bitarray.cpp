#include <gtest/gtest.h>
#include <memory>

#include <barretenberg/waffle/composer/bool_composer.hpp>
#include <barretenberg/waffle/proof_system/preprocess.hpp>
#include <barretenberg/waffle/proof_system/prover/prover.hpp>
#include <barretenberg/waffle/proof_system/verifier/verifier.hpp>
#include <barretenberg/waffle/proof_system/widgets/arithmetic_widget.hpp>

#include <barretenberg/polynomials/polynomial_arithmetic.hpp>

#include <barretenberg/waffle/stdlib/common.hpp>
#include <barretenberg/waffle/stdlib/field/field.hpp>
#include <barretenberg/waffle/stdlib/uint32/uint32.hpp>
#include <barretenberg/waffle/stdlib/bitarray/bitarray.hpp>

using namespace barretenberg;
using namespace plonk;

typedef stdlib::bool_t<waffle::BoolComposer> bool_t;
typedef stdlib::field_t<waffle::BoolComposer> field_t;
typedef stdlib::uint32<waffle::BoolComposer> uint32;
typedef stdlib::witness_t<waffle::BoolComposer> witness_t;
typedef stdlib::bitarray<waffle::BoolComposer> bitarray;

uint32_t get_random_int()
{
return static_cast<uint32_t>(barretenberg::fr::random_element().data[0]);
}


TEST(stdlib_bitarray, test_uint32_input_output_consistency)
{
    waffle::BoolComposer composer = waffle::BoolComposer();

    uint32_t a_expected = get_random_int();
    uint32_t b_expected = get_random_int();

    uint32 a = witness_t(&composer, a_expected);
    uint32 b = witness_t(&composer, b_expected);

    std::vector<uint32> inputs = { a, b };
    bitarray test_bitarray = bitarray(inputs);

    std::vector<uint32> result = test_bitarray.to_uint32_vector();

    EXPECT_EQ(result.size(), 2UL);

    uint32_t a_result = static_cast<uint32_t>(
        barretenberg::fr::from_montgomery_form(composer.get_variable(result[0].get_witness_index())).data[0]);
    uint32_t b_result = static_cast<uint32_t>(
        barretenberg::fr::from_montgomery_form(composer.get_variable(result[1].get_witness_index())).data[0]);

    EXPECT_EQ(a_result, a_expected);
    EXPECT_EQ(b_result, b_expected);
}

TEST(stdlib_bitarray, test_binary_input_output_consistency)
{
    waffle::BoolComposer composer = waffle::BoolComposer();

    bitarray test_bitarray = bitarray(&composer, 5);

    test_bitarray[0] = bool_t(witness_t(&composer, true));
    test_bitarray[1] = bool_t(witness_t(&composer, false));
    test_bitarray[2] = bool_t(witness_t(&composer, true));
    test_bitarray[3] = bool_t(witness_t(&composer, true));
    test_bitarray[4] = bool_t(witness_t(&composer, false));

    std::vector<uint32> uint32_vec = test_bitarray.to_uint32_vector();

    EXPECT_EQ(uint32_vec.size(), 1UL);

    uint32_t result = static_cast<uint32_t>(
        barretenberg::fr::from_montgomery_form(composer.get_variable(uint32_vec[0].get_witness_index())).data[0]);

    uint32_t expected = 0b01101;
    EXPECT_EQ(result, expected);
}

TEST(stdlib_bitarray, test_string_input_output_consistency)
{
    waffle::BoolComposer composer = waffle::BoolComposer();

    std::string expected = "string literals inside a SNARK circuit? What nonsense!";
    bitarray test_bitarray = bitarray(&composer, expected);

    std::string result = test_bitarray.get_witness_as_string();

    EXPECT_EQ(result, expected);
}