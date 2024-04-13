Status ShapeRefiner::PartialStridedSliceShape(
    Node* slice_node, InferenceContext* ctx, ShapeHandle* result,
    shape_inference::InferenceContext* outer_context) {
  // Only attempt to evaluate if begin/end/strides all are scalars.
  for (int i = 1; i <= 3; ++i) {
    ShapeHandle input_shape = ctx->input(i);
    if (ctx->Value(ctx->Dim(input_shape, 0)) != 1) {
      *result = ctx->UnknownShape();
      return Status::OK();
    }
  }

  int begin_mask, end_mask, ellipsis_mask, new_axis_mask, shrink_axis_mask;
  TF_RETURN_IF_ERROR(
      GetNodeAttr(slice_node->attrs(), "begin_mask", &begin_mask));
  TF_RETURN_IF_ERROR(GetNodeAttr(slice_node->attrs(), "end_mask", &end_mask));
  TF_RETURN_IF_ERROR(
      GetNodeAttr(slice_node->attrs(), "ellipsis_mask", &ellipsis_mask));
  TF_RETURN_IF_ERROR(
      GetNodeAttr(slice_node->attrs(), "new_axis_mask", &new_axis_mask));
  TF_RETURN_IF_ERROR(
      GetNodeAttr(slice_node->attrs(), "shrink_axis_mask", &shrink_axis_mask));

  // Only attempt to evaluate if there are no special masks set (note that we
  // can handle begin/end_mask == 1).
  if (!(begin_mask == 0 || begin_mask == 1) ||
      !(end_mask == 0 || end_mask == 1) || ellipsis_mask != 0 ||
      new_axis_mask != 0 || shrink_axis_mask != 0) {
    *result = ctx->UnknownShape();
    return Status::OK();
  }

  bool evaluated;
  int64 begin;
  if (begin_mask == 1) {
    begin = 0;
  } else {
    TF_RETURN_IF_ERROR(EvaluateConstantIntScalarEdge(slice_node, 1, &evaluated,
                                                     &begin, outer_context));
    if (!evaluated) {
      *result = ctx->UnknownShape();
      return Status::OK();
    }
  }

  int64 end;
  if (end_mask == 1) {
    end = std::numeric_limits<int64>::max();
  } else {
    TF_RETURN_IF_ERROR(EvaluateConstantIntScalarEdge(slice_node, 2, &evaluated,
                                                     &end, outer_context));
    if (!evaluated) {
      *result = ctx->UnknownShape();
      return Status::OK();
    }
  }

  int64 stride;
  TF_RETURN_IF_ERROR(EvaluateConstantIntScalarEdge(slice_node, 3, &evaluated,
                                                   &stride, outer_context));
  if (!evaluated) {
    *result = ctx->UnknownShape();
    return Status::OK();
  }

  // Apply stride to input interpreted as a partial shape.
  ShapeHandle input;
  TF_RETURN_IF_ERROR(
      ConstantPartialShape(ctx, slice_node, 0, &input, outer_context));
  TF_RETURN_IF_ERROR(ctx->Subshape(input, begin, end, stride, result));
  return Status::OK();
}