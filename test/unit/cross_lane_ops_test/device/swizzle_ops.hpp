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

#ifndef ROCWMMA_DEVICE_SWIZZLE_OPS_HPP
#define ROCWMMA_DEVICE_SWIZZLE_OPS_HPP

#include <rocwmma/rocwmma.hpp>

namespace rocwmma
{
    template <int ElementIdx, int SubGroupSize>
    ROCWMMA_DEVICE inline int getSwizzleBCastExpect(int input)
    {
        return (input & (~(SubGroupSize - 1))) + ElementIdx;
    }

    template <int SubGroupSize>
    ROCWMMA_DEVICE inline int getSwizzleReverseExpect(int input)
    {
        int maxInGroup = SubGroupSize - 1;
        return ((input & (~maxInGroup) | (maxInGroup - (input & maxInGroup))));
    }

    template <int SubGroupSize, int Direction, int Distance>
    ROCWMMA_DEVICE inline int getSwizzleRotateExpect(int input)
    {
        auto afterRotate = (input & (SubGroupSize - 1));
        afterRotate += Direction == CrossLaneOps::OP_DIR_L ? Distance : -Distance;
        afterRotate += SubGroupSize;
        afterRotate &= (SubGroupSize - 1);
        return (input & (~(SubGroupSize - 1))) | afterRotate;
    }

    template <uint32_t Select0, uint32_t Select1>
    ROCWMMA_DEVICE inline int getSwizzleShuffle2Expect(int input)
    {
        auto id = input & 0b1;
        input -= id;
        input += id == 0 ? Select0 : Select1;
        return input;
    }

    template <uint32_t Select0, uint32_t Select1, uint32_t Select2, uint32_t Select3>
    ROCWMMA_DEVICE inline int getSwizzleShuffle4Expect(int input)
    {
        auto id = input & 0b11;
        input -= id;
        input += id == 0 ? Select0 : id == 1 ? Select1 : id == 2 ? Select2 : Select3;
        return input;
    }

    template <uint32_t SubGroupSize>
    ROCWMMA_DEVICE inline int getSwizzleSwapExpect(int input)
    {
        return input ^ SubGroupSize;
    }

    template <typename DataT, typename CrossLaneOp>
    ROCWMMA_DEVICE std::enable_if_t<CrossLaneOp::opId() == CrossLaneOps::OP_ID_BCAST
                                        && CrossLaneOp::opImpl() == CrossLaneOps::OP_IMPL_SWIZZLE,
                                    bool>
                   swizzleOpsTestCase()
    {
        int input = threadIdx.x;
        int expect
            = getSwizzleBCastExpect<CrossLaneOp::elementIdx(), CrossLaneOp::groupSize()>(input);
        int output = rocwmma::Swizzle::Driver<CrossLaneOp>::exec(input);

        // printf("op (%d, %d), input %d, WriteRowMask %x, WriteBankMask %x, BoundCtrl %d, expect %d, output %d\n", CrossLaneOp::select0(), CrossLaneOp::select1(), input , WriteRowMask , WriteBankMask , BoundCtrl, expect , output );
        return output != expect;
    }

    template <typename DataT, typename CrossLaneOp>
    ROCWMMA_DEVICE std::enable_if_t<CrossLaneOp::opId() == CrossLaneOps::OP_ID_REVERSE
                                        && CrossLaneOp::opImpl() == CrossLaneOps::OP_IMPL_SWIZZLE,
                                    bool>
                   swizzleOpsTestCase()
    {
        int input  = threadIdx.x;
        int expect = getSwizzleReverseExpect<CrossLaneOp::groupSize()>(input);
        int output = rocwmma::Swizzle::Driver<CrossLaneOp>::exec(input);

        // printf("op (%d, %d), input %d, WriteRowMask %x, WriteBankMask %x, BoundCtrl %d, expect %d, output %d\n", CrossLaneOp::select0(), CrossLaneOp::select1(), input , WriteRowMask , WriteBankMask , BoundCtrl, expect , output );
        return output != expect;
    }

    template <typename DataT, typename CrossLaneOp>
    ROCWMMA_DEVICE std::enable_if_t<CrossLaneOp::opId() == CrossLaneOps::OP_ID_ROTATE
                                        && CrossLaneOp::opImpl() == CrossLaneOps::OP_IMPL_SWIZZLE,
                                    bool>
                   swizzleOpsTestCase()
    {
        int input  = threadIdx.x;
        int expect = getSwizzleRotateExpect<CrossLaneOp::groupSize(),
                                            CrossLaneOp::opDir(),
                                            CrossLaneOp::opDist()>(input);
        int output = rocwmma::Swizzle::Driver<CrossLaneOp>::exec(input);

        // printf("op (%d, %d), input %d, WriteRowMask %x, WriteBankMask %x, BoundCtrl %d, expect %d, output %d\n", CrossLaneOp::select0(), CrossLaneOp::select1(), input , WriteRowMask , WriteBankMask , BoundCtrl, expect , output );
        return output != expect;
    }

    template <typename DataT, typename CrossLaneOp>
    ROCWMMA_DEVICE std::enable_if_t<CrossLaneOp::opId() == CrossLaneOps::OP_ID_SHUFFLE
                                        && CrossLaneOp::opImpl() == CrossLaneOps::OP_IMPL_SWIZZLE,
                                    bool>
                   swizzleOpsTestCase()
    {
        int input  = threadIdx.x;
        int expect = -1;
        if constexpr(CrossLaneOp::groupSize() == 2)
        {
            expect
                = getSwizzleShuffle2Expect<CrossLaneOp::select0(), CrossLaneOp::select1()>(input);
        }
        else if constexpr(CrossLaneOp::groupSize() == 4)
        {
            expect = getSwizzleShuffle4Expect<CrossLaneOp::select0(),
                                              CrossLaneOp::select1(),
                                              CrossLaneOp::select2(),
                                              CrossLaneOp::select3()>(input);
        }
        int output = rocwmma::Swizzle::Driver<CrossLaneOp>::exec(input);

        // printf("op (%d, %d), input %d, WriteRowMask %x, WriteBankMask %x, BoundCtrl %d, expect %d, output %d\n", CrossLaneOp::select0(), CrossLaneOp::select1(), input , WriteRowMask , WriteBankMask , BoundCtrl, expect , output );
        return output != expect;
    }

    template <typename DataT, typename CrossLaneOp>
    ROCWMMA_DEVICE std::enable_if_t<CrossLaneOp::opId() == CrossLaneOps::OP_ID_SWAP
                                        && CrossLaneOp::opImpl() == CrossLaneOps::OP_IMPL_SWIZZLE,
                                    bool>
                   swizzleOpsTestCase()
    {
        int input  = threadIdx.x;
        int expect = getSwizzleSwapExpect<CrossLaneOp::groupSize()>(input);
        int output = rocwmma::Swizzle::Driver<CrossLaneOp>::exec(input);

        // printf("op (%d, %d), input %d, WriteRowMask %x, WriteBankMask %x, BoundCtrl %d, expect %d, output %d\n", CrossLaneOp::select0(), CrossLaneOp::select1(), input , WriteRowMask , WriteBankMask , BoundCtrl, expect , output );
        return output != expect;
    }

} // namespace rocwmma

#endif // ROCWMMA_DEVICE_SWIZZLE_OPS_HPP
