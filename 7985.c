  static Status RecordLabelToDimension(const int label, const int axis,
                                       const Tensor& input,
                                       LabelToDimSizes* label_to_dim_sizes) {
    const int64_t input_dim = input.dim_size(axis);
    // We know that label_to_dim_sizes has the size to accommodate named labels.
    if (label_to_dim_sizes->at(label) != 0 &&
        label_to_dim_sizes->at(label) != input_dim) {
      return errors::InvalidArgument(
          "Expected dimension ", label_to_dim_sizes->at(label), " at axis ",
          axis, " of the input shaped ", input.shape().DebugString(),
          " but got dimension ", input_dim);
    }
    (*label_to_dim_sizes)[label] = input_dim;
    return Status::OK();
  }