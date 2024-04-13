  static Status CopyFrom(const Tensor& input, const TensorShape& shape,
                         Tensor* output) {
    if (output->CopyFrom(input, shape)) return Status::OK();
    return errors::Internal(
        "Encountered error while reshaping a Tensor of shape ",
        input.shape().DebugString(), " to shape ", shape.DebugString());
  }