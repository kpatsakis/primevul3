Status ShapeRefiner::ConstantPartialShape(
    InferenceContext* target_context, const Node* node, int dst_idx,
    ShapeHandle* result, shape_inference::InferenceContext* outer_context) {
  const Edge* input_edge;
  TF_RETURN_IF_ERROR(node->input_edge(dst_idx, &input_edge));

  InferenceContext* src_context = GetContext(input_edge->src());
  if (src_context == nullptr) return errors::Internal("Missing src context");
  ShapeHandle src_shape = src_context->output(input_edge->src_output());

  if (src_context->Value(src_context->Rank(src_shape)) == 0) {
    Tensor t;
    bool evaluated = false;
    TF_RETURN_IF_ERROR(EvaluateConstantTensorForEdge(node, dst_idx, &evaluated,
                                                     &t, outer_context));
    if (!evaluated) {
      return errors::InvalidArgument(
          "Received a shape scalar with unknown static value.  A static value "
          "of '-1' is required to represent an unknown shape.");
    }
    if (t.dims() == 0) {
      if (t.dtype() == DT_INT32 && t.scalar<int32>()() == -1) {
        *result = target_context->UnknownShape();
        return Status::OK();
      } else if (t.dtype() == DT_INT64 && t.scalar<int64>()() == -1) {
        *result = target_context->UnknownShape();
        return Status::OK();
      }
    }
    return errors::InvalidArgument(
        "Received an invalid shape scalar with a static value that is not "
        "'-1': ",
        t.DebugString());
  }

  TF_RETURN_IF_ERROR(src_context->WithRank(src_shape, 1, &src_shape));

  const string& src_op = input_edge->src()->type_string();
  if (src_context->Value(src_context->Dim(src_shape, 0)) == 0) {
    // Source tensor is a vector of length 0, so the shape it
    // represents is as scalar.
    *result = target_context->Scalar();
  } else if (src_op == "Cast") {
    // First try to evaluate the current tensor, as it might be a valid cast of
    // a float.
    Tensor t;
    bool evaluated = false;
    if (EvaluateConstantTensorForEdge(node, dst_idx, &evaluated, &t,
                                      outer_context)
            .ok()) {
      if (evaluated &&
          target_context->MakeShapeFromTensor(&t, src_shape, result).ok()) {
        return Status::OK();
      }
    }

    // Then try to infer partial shape from the input to the cast tensor.
    ShapeHandle pre_cast_shape;
    if (!ConstantPartialShape(target_context, input_edge->src(), 0,
                              &pre_cast_shape, outer_context)
             .ok()) {
      TF_RETURN_IF_ERROR(
          target_context->MakeShapeFromTensor(nullptr, src_shape, result));
    }
    if (!target_context->RankKnown(pre_cast_shape)) {
      // Failed to evaluate. Treat the output as completely unknown.
      *result = target_context->UnknownShape();
      return Status::OK();
    }
    auto* dest_type = input_edge->src()->attrs().Find("DstT");
    if (dest_type == nullptr || dest_type->value_case() != AttrValue::kType ||
        (dest_type->type() != DT_INT32 && dest_type->type() != DT_INT64)) {
      // Casting to a weird type. Do not attempt to infer across it.
      *result = target_context->MakeShape(std::vector<DimensionHandle>(
          target_context->Rank(pre_cast_shape), target_context->UnknownDim()));
      return Status::OK();
    }
    *result = pre_cast_shape;
  } else if (src_op == "Shape") {
    *result = src_context->input(0);
  } else if (src_op == "ShapeN") {
    *result = src_context->input(input_edge->src_output());
  } else if (src_op == "Pack") {
    std::vector<DimensionHandle> dims;
    // Pack is concatenating its input scalars to form the shape tensor vector.
    for (int i = 0; i < src_context->num_inputs(); ++i) {
      int64 size;
      bool evaluated;
      TF_RETURN_IF_ERROR(EvaluateConstantIntScalarEdge(
          input_edge->src(), i, &evaluated, &size, outer_context));
      if (evaluated) {
        dims.push_back(size < 0 ? target_context->UnknownDim()
                                : target_context->MakeDim(size));
      } else {
        dims.push_back(target_context->UnknownDim());
      }
    }
    *result = target_context->MakeShape(dims);
  } else if (src_op == "Concat" || src_op == "ConcatV2") {
    *result = target_context->Scalar();
    // For Concat, input 0 is concat dim; for V2 it is the last input.
    const int concat_dim =
        src_op == "Concat" ? 0 : src_context->num_inputs() - 1;
    // Concat is concatenating its input shape vectors.
    for (int i = 0; i < src_context->num_inputs(); ++i) {
      // Concat dim is ignored (and will always be a scalar).
      if (i == concat_dim) continue;
      ShapeHandle sub_result;
      TF_RETURN_IF_ERROR(ConstantPartialShape(target_context, input_edge->src(),
                                              i, &sub_result, outer_context));
      if (!target_context->RankKnown(sub_result)) {
        // Failed to evaluate. Treat the output as completely unknown.
        // TODO(cwhipkey): we could rely on all inputs being the same rank, so
        // figure that rank out and append the right number of unknown dims.
        *result = target_context->UnknownShape();
        return Status::OK();
      }
      TF_RETURN_IF_ERROR(
          target_context->Concatenate(*result, sub_result, result));
    }
  } else if (src_op == "StridedSlice") {
    TF_RETURN_IF_ERROR(PartialStridedSliceShape(input_edge->src(), src_context,
                                                result, outer_context));
  } else if (src_op == "VariableShape") {
    auto* handle_data = src_context->input_handle_shapes_and_types(0);
    if (handle_data != nullptr && !handle_data->empty()) {
      *result = handle_data->at(0).shape;
    } else {
      *result = target_context->UnknownShape();
    }
  } else {
    Tensor t;
    bool evaluated = false;
    TF_RETURN_IF_ERROR(EvaluateConstantTensorForEdge(node, dst_idx, &evaluated,
                                                     &t, outer_context));
    TF_RETURN_IF_ERROR(target_context->MakeShapeFromTensor(
        evaluated ? &t : nullptr, src_shape, result));
  }
  return Status::OK();
}