  void Compute(OpKernelContext* ctx) override {
    OpInputList inputs;
    OP_REQUIRES_OK(ctx, ctx->input_list("inputs", &inputs));

    OperandLabels input_labels(input_labels_);
    Labels output_labels(output_labels_);
    std::vector<EinsumHelper::DimensionType> label_types(label_types_);
    OperandLabelCounts input_label_counts(input_label_counts_);
    LabelCounts output_label_counts(output_label_counts_);
    LabelToDimSizes label_to_dim_sizes;

    OP_REQUIRES_OK(ctx, EinsumHelper::ProcessDimensions(
                            inputs, input_has_ellipsis_, output_has_ellipsis_,
                            &input_labels, &output_labels, &label_types,
                            &input_label_counts, &output_label_counts,
                            &label_to_dim_sizes));

    // The reduction phase (a) sums across reduction dimensions, (b) takes
    // generalized diagonals, and (c) reshapes it into shape
    //   [(broadcasting) batch shape] + [F,C]
    // where F and C denote the total (compacted) size of free and contract
    // dimensions, respectively.
    const int num_inputs = inputs.size();
    OperandLabels free_labels(num_inputs);
    gtl::InlinedVector<Tensor, 2> inputs_reduced(num_inputs);
    gtl::InlinedVector<bool, 2> swap_free_and_contract(num_inputs);
    for (int i = 0; i < num_inputs; ++i) {
      OP_REQUIRES_OK(ctx,
                     EinsumHelper::ReduceOperand<Device, T>(
                         ctx, inputs[i], label_types, input_label_counts[i],
                         &input_labels[i], &free_labels[i],
                         &swap_free_and_contract[i], &inputs_reduced[i]));
    }

    // After reduction, the inputs should be reshaped to Tensors suitable for
    // contraction. If num_inputs is 1, the reduced input is simply forwarded to
    // the output.
    Tensor contraction_output_reshaped;
    OP_REQUIRES_OK(ctx, EinsumHelper::ContractOperands<Device, T>(
                            ctx, inputs_reduced, swap_free_and_contract,
                            &contraction_output_reshaped));

    // Copy the batch labels from the contraction output. Recover the batch
    // shape, which may have been broadcasted.
    TensorShape result_shape = contraction_output_reshaped.shape();
    result_shape.RemoveLastDims(2);

    int num_labels = label_types.size();
    Labels result_labels;
    // All batch dimensions should be present in the contracted result. First
    // the broadcasting dimensions, then the named batch dimensions.
    for (int label = 0; label < num_labels; ++label) {
      if (label_types[label] == EinsumHelper::kBroadcasting)
        result_labels.push_back(label);
    }
    for (int label = 0; label < num_labels; ++label) {
      if (label_types[label] == EinsumHelper::kBatch)
        result_labels.push_back(label);
    }
    for (int i = 0; i < num_inputs; ++i) {
      for (int label : free_labels[i]) {
        result_labels.push_back(label);
        result_shape.AddDim(label_to_dim_sizes[label]);
      }
    }

    // Reshape the contraction (or reduction) result to its expanded shape:
    // [(broadcasted) batch shape] + [free shape 0] + [free shape 1].
    Tensor contraction_output;
    OP_REQUIRES_OK(
        ctx, EinsumHelper::CopyFrom(contraction_output_reshaped, result_shape,
                                    &contraction_output));

    // Inflate the output if necessary. (E.g. for the equation 'i->iii' which
    // may arise while computing gradient of a regular Einsum).
    // TODO(anudhyan): It's possible that Eigen's contract and inflate can be
    // chained here to avoid materializing an intermediate.
    Tensor output_inflated;
    OP_REQUIRES_OK(
        ctx, EinsumHelper::StrideOrInflate<Device, T>(
                 ctx, contraction_output, result_labels, output_label_counts,
                 true /* should_inflate */, &output_inflated));
    if (output_inflated.dims() > contraction_output.dims()) {
      // We inflated the output. Modify result labels accordingly.
      Labels inflated_labels;
      for (int label : result_labels) {
        inflated_labels.insert(inflated_labels.end(),
                               output_label_counts[label], label);
      }
      result_labels.swap(inflated_labels);
    }
    // Find the permutation to map the result labels to the output labels. Note
    // that both the result and the final output may have the repeated labels,
    // in which case the permutation preserves the left-to-right ordering.
    // E.g. if result labels are [0, 0, 1] and output is [0, l, 0] then the
    // permutation should be [0, 2, 1]. We also use the fact that repeated
    // labels in the result are adjacent to each other.
    std::vector<int> output_permutation(output_labels.size());
    std::vector<int> label_to_position(num_labels, -1);
    for (int i = 0; i < result_labels.size(); ++i) {
      // Remember the position of only the leftmost result label.
      if (label_to_position[result_labels[i]] == -1) {
        label_to_position[result_labels[i]] = i;
      }
    }
    for (int i = 0; i < output_labels.size(); ++i) {
      output_permutation[i] = label_to_position[output_labels[i]];
      // We have found the leftmost occurrence. The next one would be adjacent.
      label_to_position[output_labels[i]] += 1;
    }
    Tensor output;
    OP_REQUIRES_OK(ctx, EinsumHelper::TransposeOperand<Device, T>(
                            ctx, output_inflated, output_permutation, &output));
    ctx->set_output(0, output);
  }