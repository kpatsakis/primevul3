Status ShapeRefiner::EvaluateConstantIntScalarEdge(
    const Node* node, int dst_idx, bool* evaluated, int64* result,
    shape_inference::InferenceContext* outer_context) {
  Tensor scalar;
  TF_RETURN_IF_ERROR(EvaluateConstantTensorForEdge(node, dst_idx, evaluated,
                                                   &scalar, outer_context));
  if (*evaluated) {
    if (scalar.NumElements() != 1) {
      return errors::InvalidArgument(
          "EvaluateConstantIntScalarEdge called on non-scalar edge: ",
          scalar.NumElements());
    }
    if (scalar.dtype() == DT_INT32) {
      *result = scalar.scalar<int32>()();
    } else {
      if (scalar.dtype() != DT_INT64) {
        return errors::InvalidArgument(
            "EvaluateConstantIntScalarEdge called on non-integer edge: ",
            scalar.dtype());
      }
      *result = scalar.scalar<int64>()();
    }
  }
  return Status::OK();
}