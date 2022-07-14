/*******************************************************************************
 *
 * MIT License
 *
 * Copyright 2021-2022 Advanced Micro Devices, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 *******************************************************************************/

#ifndef ROCWMMA_GEMM_COMMON_TEST_PARAMS_HPP
#define ROCWMMA_GEMM_COMMON_TEST_PARAMS_HPP

#include <tuple>
#include <vector>

#include <rocwmma/internal/types.hpp>

#include "common.hpp"
#include "gemm_kernel_base.hpp"
#include "kernel_generator.hpp"

namespace rocwmma
{
    ///
    /// Generalized kernel params for most tests
    ///
    struct CommonTestParams
    {
        ///
        /// Testing types as Input/Output/Compute (IOC)
        ///

        // Native int8
        using TestTypesI8 = std::tuple<
#if defined(ROCWMMA_EXTENDED_TESTS)
            std::tuple<int8_t, int8_t, int32_t>,
#endif // ROCWMMA_EXTENDED_TESTS
            std::tuple<int8_t, int32_t, int32_t>>;

        // Non-native bfloat16_t
        using TestTypesBF16 = std::tuple<
#if defined(ROCWMMA_EXTENDED_TESTS)
            std::tuple<bfloat16_t, bfloat16_t, bfloat16_t>,
            std::tuple<bfloat16_t, bfloat16_t, float32_t>,
#endif // ROCWMMA_EXTENDED_TESTS
            std::tuple<bfloat16_t, float32_t, float32_t>>;

        // Native f16
        using TestTypesF16 = std::tuple<
#if defined(ROCWMMA_EXTENDED_TESTS)
            std::tuple<float16_t, float16_t, float16_t>,
            std::tuple<float16_t, float16_t, float32_t>,
#endif // ROCWMMA_EXTENDED_TESTS
            std::tuple<float16_t, float32_t, float32_t>>;

        // Non-native hfloat16_t (i.e. __half)
        using TestTypesH16 = std::tuple<
#if defined(ROCWMMA_EXTENDED_TESTS)
            std::tuple<hfloat16_t, hfloat16_t, hfloat16_t>,
            std::tuple<hfloat16_t, hfloat16_t, float32_t>,
#endif // ROCWMMA_EXTENDED_TESTS
            std::tuple<hfloat16_t, float32_t, float32_t>>;

        // Native single f32
        using TestTypesF32 = std::tuple<std::tuple<float32_t, float32_t, float32_t>>;

        // Native double f64
        using TestTypesF64 = std::tuple<std::tuple<float64_t, float64_t, float64_t>>;

        // Aggregate types <= 16 bit
        using TestTypesSmall =
            typename Concat<TestTypesI8, TestTypesBF16, TestTypesF16, TestTypesH16>::Result;

        // Aggregate types <= 32 bit
        using TestTypesMedium = typename Concat<TestTypesSmall, TestTypesF32>::Result;

        // Aggregate types <= 64 bit
        using TestTypesLarge = typename Concat<TestTypesMedium, TestTypesF64>::Result;

        // 16 x 16 supports up to f64
        using TestTypes16x16 = TestTypesLarge;

        // 32 x 32 supports up to f32
        using TestTypes32x32 = TestTypesMedium;

        ///
        /// Data Layout types: col_major (N) or row_major (T)
        /// Matrices layouts: A / B / C / D
        /// Lds Layouts: Lds data
        ///

        // Supported generalized data layouts
        using TestDataLayouts = std::tuple<row_major, col_major>;

        // Lds data layouts
        using TestLdsDataLayouts = TestDataLayouts;

        // Aggregate data layout combinations A / B / CD
        // Note: for the following sets, assume C = D
        using TestLayoutsNN =
            typename CombineOne<std::tuple<col_major, col_major>, TestDataLayouts>::Result;
        using TestLayoutsNT =
            typename CombineOne<std::tuple<col_major, row_major>, TestDataLayouts>::Result;
        using TestLayoutsTN =
            typename CombineOne<std::tuple<row_major, col_major>, TestDataLayouts>::Result;
        using TestLayoutsTT =
            typename CombineOne<std::tuple<row_major, row_major>, TestDataLayouts>::Result;

        ///
        /// MFMA block sizes
        /// BlockK variances for particular BlockM, BlockN
        ///

        using TestBlockSizes16x16 = std::tuple<std::tuple<I<16>, I<16>, I<16>>,
                                               std::tuple<I<16>, I<16>, I<32>>,
                                               std::tuple<I<16>, I<16>, I<64>>
#if defined(ROCWMMA_EXTENDED_TESTS)
                                               ,
                                               std::tuple<I<16>, I<16>, I<128>>,
                                               std::tuple<I<16>, I<16>, I<256>>
#endif // ROCWMMA_EXTENDED_TESTS
                                               >;

        using TestBlockSizes32x32 = std::tuple<std::tuple<I<32>, I<32>, I<8>>,
                                               std::tuple<I<32>, I<32>, I<16>>,
                                               std::tuple<I<32>, I<32>, I<32>>
#if defined(ROCWMMA_EXTENDED_TESTS)
                                               ,
                                               std::tuple<I<32>, I<32>, I<64>>,
                                               std::tuple<I<32>, I<32>, I<128>>
#endif // ROCWMMA_EXTENDED_TESTS
                                               >;

        ///
        /// Run-time kernel argument parameters
        ///

        // Types of parameters
        using KernelT      = std::shared_ptr<KernelI>; // Kernel test interface
        using ThreadBlockT = std::pair<int64_t, int64_t>;
        using ProblemSizeT = std::tuple<int64_t, int64_t, int64_t>;
        using AlphaT       = float64_t;
        using BetaT        = float64_t;

        static inline std::vector<ThreadBlockT> threadBlocks()
        {
            return
            {
                // clang-format off
                // Don't benchmark wg less than 4 waves by default
#if defined(ROCWMMA_VALIDATION_TESTS) || defined(ROCWMMA_EXTENDED_TESTS)
                {64, 1}, // 1 wave
                {64, 2}, {128, 1}, // 2 wave
#endif // ROCWMMA_VALIDATION_TESTS
                {64, 4}, {128, 2}, // 4 wave
                {256, 1}  // 4 wave
                // clang-format on
            };
        }

        static inline std::vector<ProblemSizeT> problemSizes()
        {

            return
            {
                // clang-format off
                {64, 64, 1024},
                {32, 64, 1024},
                {64, 32, 1024},
                {256, 256, 1024},
                {2048, 64, 1024},
                {64, 2048, 1024},
                {1024, 1024, 1024},
                // Skip validation on larger sizes
                // due to very slow.
#if !defined(ROCWMMA_VALIDATION_TESTS)
                {2048, 2048, 2048},
                {2560, 2560, 2560},
                {3072, 3072, 3072},
                {3584, 3584, 3584},
                {4096, 4096, 4096},
                {5120, 5120, 5120},
#ifdef ROCWMMA_EXTENDED_TESTS
                {6144, 6144, 6144},
                {7168, 7168, 7168},
                {8192, 8192, 8192},
#endif // ROCWMMA_EXTENDED_TESTS
#endif // !ROCWMMA_VALIDATION_TESTS \
    // clang-format on
            };
        }

        static inline std::vector<AlphaT> alphas()
        {
            return {2.0};
        }

        static inline std::vector<BetaT> betas()
        {
            return {2.0};
        }
    };

} // namespace rocwmma

///
/// Test suite instantiation
///
#define ROCWMMA_INSTANTIATE_GTEST_SUITE(test_suite_prefix, test_suite_name)          \
    class test_suite_name : public rocwmma::GemmTest                                 \
    {                                                                                \
    };                                                                               \
                                                                                     \
    TEST_P(test_suite_name, RunKernel)                                               \
    {                                                                                \
        this->RunKernel();                                                           \
    }                                                                                \
                                                                                     \
    INSTANTIATE_TEST_SUITE_P(                                                        \
        test_suite_prefix,                                                           \
        test_suite_name,                                                             \
        ::testing::Combine(::testing::ValuesIn(rocwmma::TestParams::kernels()),      \
                           ::testing::ValuesIn(rocwmma::TestParams::threadBlocks()), \
                           ::testing::ValuesIn(rocwmma::TestParams::problemSizes()), \
                           ::testing::ValuesIn(rocwmma::TestParams::alphas()),       \
                           ::testing::ValuesIn(rocwmma::TestParams::betas())));

#define ROCWMMA_INSTANTIATE_GTEST_SUITE_NO_WARMUP(test_suite_prefix, test_suite_name) \
    class test_suite_name : public rocwmma::GemmTest                                  \
    {                                                                                 \
    };                                                                                \
                                                                                      \
    TEST_P(test_suite_name, RunKernel)                                                \
    {                                                                                 \
        this->RunKernelWithoutWarmup();                                               \
    }                                                                                 \
                                                                                      \
    INSTANTIATE_TEST_SUITE_P(                                                         \
        test_suite_prefix,                                                            \
        test_suite_name,                                                              \
        ::testing::Combine(::testing::ValuesIn(rocwmma::TestParams::kernels()),       \
                           ::testing::ValuesIn(rocwmma::TestParams::threadBlocks()),  \
                           ::testing::ValuesIn(rocwmma::TestParams::problemSizes()),  \
                           ::testing::ValuesIn(rocwmma::TestParams::alphas()),        \
                           ::testing::ValuesIn(rocwmma::TestParams::betas())));

#endif // ROCWMMA_GEMM_COMMON_TEST_PARAMS_HPP
