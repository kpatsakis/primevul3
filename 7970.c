  static void InsertBroadcastLabels(int num_bcast_dims, int num_named_labels,
                                    int ellipsis_axis, Labels* labels,
                                    LabelCounts* label_counts) {
    labels->erase(labels->begin() + ellipsis_axis);
    labels->insert(labels->begin() + ellipsis_axis, num_bcast_dims, 0);
    std::iota(labels->begin() + ellipsis_axis,
              labels->begin() + ellipsis_axis + num_bcast_dims,
              num_named_labels);
    // Increment label counts. Since these are new labels, the count is set
    // to 1.
    label_counts->resize(num_named_labels + num_bcast_dims, 1);
  }