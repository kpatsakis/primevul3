  static Status ReshapeToRank3(const Tensor& input, int batch_size,
                               Tensor* output) {
    const int rank = input.dims();
    TensorShape output_shape = {batch_size, input.dim_size(rank - 2),
                                input.dim_size(rank - 1)};
    return CopyFrom(input, output_shape, output);
  }