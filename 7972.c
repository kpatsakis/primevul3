  static Status ReduceOperand(OpKernelContext* ctx, const Tensor& input,
                              const std::vector<DimensionType>& label_types,
                              const LabelCounts& label_counts, Labels* labels,
                              Labels* free_labels, bool* swap_free_and_contract,
                              Tensor* output) {
    // Find the permutation to transpose the input dimensions in the order of
    // DimensionType; i.e. batch, free, contract and reduce dimensions. This
    // makes it more convenient to invoke Reduce/Contract operations.
    std::vector<int> permutation(input.dims());
    absl::c_iota(permutation, 0);
    Tensor input_transposed;
    // Check if we can avoid the transpose. We need to flip the adj_x (or adj_y)
    // flag during BatchMatMul. This is an extra optimization not necessary for
    // correctness.
    if (ShouldSwapFreeAndContract(*labels, label_types)) {
      *swap_free_and_contract = true;
    } else {
      absl::c_sort(permutation, [&](int i, int j) {
        int label_i = (*labels)[i];
        int label_j = (*labels)[j];
        return std::tie(label_types[label_i], label_i) <
               std::tie(label_types[label_j], label_j);
      });
    }
    // Transpose the input so that DimensionTypes are in order.
    TF_RETURN_IF_ERROR(TransposeOperand<Device, T>(ctx, input, permutation,
                                                   &input_transposed));
    PermuteLabels(permutation, labels);

    // Take the generalized diagonal for dimensions with repeated axis labels.
    Tensor input_deduped;
    labels->erase(std::unique(labels->begin(), labels->end()), labels->end());
    TF_RETURN_IF_ERROR(
        StrideOrInflate<Device, T>(ctx, input_transposed, *labels, label_counts,
                                   false /* should_inflate */, &input_deduped));

    // Reshape denotes the rank-5 shape [broadcast, batch, free, contract,
    // reduce] where we've compacted the dimensions of each DimensionType.
    gtl::InlinedVector<int64_t, 5> reshape(5, 1);
    // The output shape is [batch shape] + [free size, contract size]
    // That is, the batch shape is preserved (for broadcasting while
    // contracting) while the free dims and contract dims are compressed to one
    // dimension each.
    TensorShape output_shape;
    for (int label_idx = 0; label_idx < labels->size(); ++label_idx) {
      const int label = labels->at(label_idx);
      int64_t dim = input_deduped.dim_size(label_idx);
      if (label_types[label] == kBroadcasting || label_types[label] == kBatch) {
        output_shape.AddDim(dim);
      } else if (label_types[label] == kFree) {
        free_labels->push_back(label);
      }
      reshape[label_types[label]] *= dim;
    }
    if (*swap_free_and_contract) std::swap(reshape[kFree], reshape[kContract]);
    output_shape.AddDim(reshape[kFree]);
    output_shape.AddDim(reshape[kContract]);

    if (reshape[kReduce] == 1) {  // No need to actually reduce.
      return CopyFrom(input_deduped, output_shape, output);
    }
    TF_RETURN_IF_ERROR(
        ctx->allocate_temp(DataTypeToEnum<T>::value, output_shape, output));
    using Reducer = Eigen::internal::SumReducer<T>;
    using Index = typename TTypes<T>::Tensor::Index;
    // Reduce along the last axis (i.e axis 1) of the rank-2 Tensor.
    const int64_t output_size = reshape[kBroadcasting] * reshape[kBatch] *
                                reshape[kFree] * reshape[kContract];
    functor::ReduceFunctor<Device, Reducer>::Reduce(
        ctx, output->shaped<T, 1>({output_size}),
        const_cast<const Tensor&>(input_deduped)
            .shaped<T, 2>({output_size, reshape[kReduce]}),
        Eigen::array<Index, 1>({1}), Reducer());
    return Status::OK();
  }