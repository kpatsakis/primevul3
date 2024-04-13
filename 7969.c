  static Status TransposeOperand(OpKernelContext* ctx, const Tensor& input,
                                 const std::vector<int>& permutation,
                                 Tensor* output) {
    if (!ShouldTranspose(input.shape(), permutation)) {
      return CopyFrom(input, input.shape(), output);
    }
    TensorShape transposed_shape;
    for (int i = 0; i < input.dims(); ++i) {
      transposed_shape.AddDim(input.dim_size(permutation[i]));
    }
    // For empty Tensors, just change the shape. E.g. we may need to transpose
    // from shape [1, 0, 5] to [5, 1, 0].
    if (input.NumElements() == 0) {
      return CopyFrom(input, transposed_shape, output);
    }
    TF_RETURN_IF_ERROR(
        ctx->allocate_temp(DataTypeToEnum<T>::value, transposed_shape, output));
    const Device& device = ctx->eigen_device<Device>();
    TF_RETURN_IF_ERROR(DoTranspose(device, input, permutation, output));
    return Status::OK();
  }