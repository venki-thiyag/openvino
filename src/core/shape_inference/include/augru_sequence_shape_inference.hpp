// Copyright (C) 2018-2022 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//
#pragma once

#include "gru_sequence_shape_inference.hpp"
#include "ngraph_ops/augru_sequence.hpp"
#include "utils.hpp"

namespace ov {
namespace op {

namespace internal {
template <class ShapeType>
void shape_infer(const ov::op::internal::AUGRUSequence* op,
                 const std::vector<ShapeType>& input_shapes,
                 std::vector<ShapeType>& output_shapes) {
    constexpr size_t expected_in_shapes_count = 7;
    NODE_VALIDATION_CHECK(op,
                          input_shapes.size() == expected_in_shapes_count,
                          "Incorrect number of input shapes has been provided. Expected: ",
                          expected_in_shapes_count,
                          ", got: ",
                          input_shapes.size(),
                          ".");

    rnn::gru_sequence_shape_infer(op, input_shapes, output_shapes);

    // A input shape validation // [batch_size, seq_length, 1]
    const auto& a_shape = input_shapes.back();
    const auto& x_shape = input_shapes[0];
    NODE_VALIDATION_CHECK(op, a_shape.rank().compatible(3), "'A' input must be a 3D tensor.");
    if (a_shape.rank().is_static()) {
        if (x_shape.rank().is_static()) {
            NODE_VALIDATION_CHECK(op,
                                  x_shape.rank().get_length() > 1 && a_shape[0].compatible(x_shape[0]),
                                  "Dimension `batch_size` must be the same for `X` and `A` inputs.");
            NODE_VALIDATION_CHECK(op,
                                  x_shape.rank().get_length() > 2 && a_shape[1].compatible(x_shape[1]),
                                  "Dimension `seq_length` must be the same for `X` and `A` inputs.");
        }
        NODE_VALIDATION_CHECK(op, a_shape[2].compatible(1), "The last dimension of `A` shape must be equal to `1`.");
    }
}
}  // namespace internal
}  // namespace op
}  // namespace ov
