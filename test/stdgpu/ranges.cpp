/*
 *  Copyright 2020 Patrick Stotko
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include <gtest/gtest.h>

#include <thrust/copy.h>
#include <thrust/functional.h>
#include <thrust/tabulate.h>

#include <stdgpu/iterator.h>
#include <stdgpu/memory.h>
#include <stdgpu/platform.h>
#include <stdgpu/ranges.h>



class stdgpu_ranges : public ::testing::Test
{
    protected:
        // Called before each test
        virtual void SetUp()
        {

        }

        // Called after each test
        virtual void TearDown()
        {

        }

};


// Explicit template instantiations
namespace stdgpu
{

template
class device_range<int>;

template
class host_range<int>;

template
class transform_range<device_range<int>, thrust::identity<int>>;

} // namespace stdgpu


TEST_F(stdgpu_ranges, device_range_with_size)
{
    const stdgpu::index_t size = 42;
    int* array = createDeviceArray<int>(size);

    stdgpu::device_range<int> array_range(array, size);
    int* array_begin   = array_range.begin().get();
    int* array_end     = array_range.end().get();

    EXPECT_EQ(array_begin, array);
    EXPECT_EQ(array_end,   array + size);

    destroyDeviceArray<int>(array);
}


TEST_F(stdgpu_ranges, device_range_automatic_size)
{
    const stdgpu::index_t size = 42;
    int* array = createDeviceArray<int>(size);

    stdgpu::device_range<int> array_range(array);
    int* array_begin   = array_range.begin().get();
    int* array_end     = array_range.end().get();

    EXPECT_EQ(array_begin, array);
    EXPECT_EQ(array_end,   array + size);

    destroyDeviceArray<int>(array);
}


TEST_F(stdgpu_ranges, host_range_with_size)
{
    const stdgpu::index_t size = 42;
    int* array = createHostArray<int>(size);

    stdgpu::host_range<int> array_range(array, size);
    int* array_begin   = array_range.begin().get();
    int* array_end     = array_range.end().get();

    EXPECT_EQ(array_begin, array);
    EXPECT_EQ(array_end,   array + size);

    destroyHostArray<int>(array);
}


TEST_F(stdgpu_ranges, host_range_automatic_size)
{
    const stdgpu::index_t size = 42;
    int* array = createHostArray<int>(size);

    stdgpu::host_range<int> array_range(array);
    int* array_begin   = array_range.begin().get();
    int* array_end     = array_range.end().get();

    EXPECT_EQ(array_begin, array);
    EXPECT_EQ(array_end,   array + size);

    destroyHostArray<int>(array);
}


template <typename T>
struct square
{
    STDGPU_HOST_DEVICE T
    operator()(const T x) const
    {
        return x * x;
    }
};


TEST_F(stdgpu_ranges, transform_range)
{
    const stdgpu::index_t size = 42;
    int* array          = createHostArray<int>(size);
    int* array_result   = createHostArray<int>(size);

    stdgpu::host_range<int> array_range(array);
    stdgpu::transform_range<stdgpu::host_range<int>, square<int>> square_range(array_range, square<int>());

    // Setup array
    thrust::tabulate(array_range.begin(), array_range.end(),
                     thrust::identity<int>());

    // Execute transformation and write into array_result
    thrust::copy(square_range.begin(), square_range.end(),
                 stdgpu::host_begin(array_result));

    for (stdgpu::index_t i = 0; i < size; ++i)
    {
        EXPECT_EQ(array[i], i);
        EXPECT_EQ(array_result[i], i * i);
    }

    destroyHostArray<int>(array);
    destroyHostArray<int>(array_result);
}


