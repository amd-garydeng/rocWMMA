/*******************************************************************************
 *
 * MIT License
 *
 * Copyright (C) 2021-2024 Advanced Micro Devices, Inc. All rights reserved.
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

#ifndef ROCWMMA_UTILITY_NUMERIC_LIMITS_HPP
#define ROCWMMA_UTILITY_NUMERIC_LIMITS_HPP

#if defined(__HIPCC_RTC__)

#include "numeric_limits_impl.hpp"
namespace rocwmma
{
    // Use drop-in replacement
    using detail::numeric_limits;

} // namespace rocwmma

#define ROCWMMA_NUMERIC_LIMITS_IMPL_NAMESPACE rocwmma::detail

#else

#include <limits>
namespace rocwmma
{
    // Use STL
    using std::numeric_limits;

} // namespace rocwmma

#define ROCWMMA_NUMERIC_LIMITS_IMPL_NAMESPACE std

#endif // defined(__HIPCC_RTC__)

#endif // ROCWMMA_UTILITY_NUMERIC_LIMITS_HPP
