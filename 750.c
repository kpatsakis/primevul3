  void Compute(OpKernelContext* context) override {
    // boxes: [batch_size, num_anchors, q, 4]
    const Tensor& boxes = context->input(0);
    // scores: [batch_size, num_anchors, num_classes]
    const Tensor& scores = context->input(1);
    OP_REQUIRES(
        context, (boxes.dim_size(0) == scores.dim_size(0)),
        errors::InvalidArgument("boxes and scores must have same batch size"));

    // max_output_size: scalar
    const Tensor& max_output_size = context->input(2);
    OP_REQUIRES(
        context, TensorShapeUtils::IsScalar(max_output_size.shape()),
        errors::InvalidArgument("max_size_per_class must be 0-D, got shape ",
                                max_output_size.shape().DebugString()));
    const int max_size_per_class = max_output_size.scalar<int>()();
    // max_total_size: scalar
    const Tensor& max_total_size = context->input(3);
    OP_REQUIRES(
        context, TensorShapeUtils::IsScalar(max_total_size.shape()),
        errors::InvalidArgument("max_total_size must be 0-D, got shape ",
                                max_total_size.shape().DebugString()));
    const int max_total_size_per_batch = max_total_size.scalar<int>()();
    OP_REQUIRES(context, max_total_size_per_batch > 0,
                errors::InvalidArgument("max_total_size must be > 0"));
    // Throw warning when `max_total_size` is too large as it may cause OOM.
    if (max_total_size_per_batch > pow(10, 6)) {
      LOG(WARNING) << "Detected a large value for `max_total_size`. This may "
                   << "cause OOM error. (max_total_size: "
                   << max_total_size.scalar<int>()() << ")";
    }
    // iou_threshold: scalar
    const Tensor& iou_threshold = context->input(4);
    OP_REQUIRES(context, TensorShapeUtils::IsScalar(iou_threshold.shape()),
                errors::InvalidArgument("iou_threshold must be 0-D, got shape ",
                                        iou_threshold.shape().DebugString()));
    const float iou_threshold_val = iou_threshold.scalar<float>()();

    // score_threshold: scalar
    const Tensor& score_threshold = context->input(5);
    OP_REQUIRES(
        context, TensorShapeUtils::IsScalar(score_threshold.shape()),
        errors::InvalidArgument("score_threshold must be 0-D, got shape ",
                                score_threshold.shape().DebugString()));
    const float score_threshold_val = score_threshold.scalar<float>()();

    OP_REQUIRES(context, iou_threshold_val >= 0 && iou_threshold_val <= 1,
                errors::InvalidArgument("iou_threshold must be in [0, 1]"));
    int num_boxes = 0;
    const int num_classes = scores.dim_size(2);
    ParseAndCheckCombinedNMSBoxSizes(context, boxes, &num_boxes, num_classes);
    CheckCombinedNMSScoreSizes(context, num_boxes, scores);

    if (!context->status().ok()) {
      return;
    }
    BatchedNonMaxSuppressionOp(context, boxes, scores, num_boxes,
                               max_size_per_class, max_total_size_per_batch,
                               score_threshold_val, iou_threshold_val,
                               pad_per_class_, clip_boxes_);
  }