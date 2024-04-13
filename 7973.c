  static Status ContractOperands(OpKernelContext* ctx,
                                 absl::Span<const Tensor> inputs,
                                 absl::Span<const bool> swap_free_and_contract,
                                 Tensor* output) {
    if (inputs.size() == 1)
      return CopyFrom(inputs[0], inputs[0].shape(), output);
    MatMulBCast bcast(inputs[0].shape().dim_sizes(),
                      inputs[1].shape().dim_sizes());
    if (!bcast.IsValid()) {
      return errors::InvalidArgument(
          "Invalid broadcasting dimensions: ", inputs[0].shape().DebugString(),
          " vs. ", inputs[1].shape().DebugString());
    }
    Tensor lhs;
    TF_RETURN_IF_ERROR(ReshapeToRank3(inputs[0], bcast.x_batch_size(), &lhs));
    Tensor rhs;
    TF_RETURN_IF_ERROR(ReshapeToRank3(inputs[1], bcast.y_batch_size(), &rhs));
    TensorShape output_shape = bcast.output_batch_shape();
    for (int i = 0; i < inputs.size(); ++i) {
      const int64_t free_axis =
          inputs[i].dims() - (swap_free_and_contract[i] ? 1 : 2);
      output_shape.AddDim(inputs[i].dim_size(free_axis));
    }
    bool trans_x = swap_free_and_contract[0];
    bool trans_y = !swap_free_and_contract[1];
    TF_RETURN_IF_ERROR(
        ctx->allocate_temp(DataTypeToEnum<T>::value, output_shape, output));
    if (lhs.NumElements() == 0 || rhs.NumElements() == 0) {
      functor::SetZeroFunctor<Device, T> set_zero;
      set_zero(ctx->eigen_device<Device>(), output->flat<T>());
      return Status::OK();
    }
    Tensor output_reshaped;
    TF_RETURN_IF_ERROR(
        ReshapeToRank3(*output, bcast.output_batch_size(), &output_reshaped));
    LaunchBatchMatMul<Device, T>::Launch(ctx, lhs, rhs, /*adj_x=*/false,
                                         /*adj_y=*/false, trans_x, trans_y,
                                         bcast, &output_reshaped);
    return Status::OK();
  }