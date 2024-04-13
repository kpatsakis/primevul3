  explicit EinsumOp(OpKernelConstruction* c) : OpKernel(c) {
    OP_REQUIRES_OK(c, c->GetAttr("equation", &equation_));
    OP_REQUIRES_OK(
        c, EinsumHelper::ParseEquation(
               equation_, &input_labels_, &output_labels_, &label_types_,
               &input_label_counts_, &output_label_counts_,
               &input_has_ellipsis_, &output_has_ellipsis_));
  }