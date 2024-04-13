  explicit MklRequantizationRangePerChannelOp(OpKernelConstruction* ctx)
      : OpKernel(ctx) {
    OP_REQUIRES_OK(ctx, ctx->GetAttr("clip_value_max", &clip_value_max_));
  }