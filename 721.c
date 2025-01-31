  void Compute(OpKernelContext* const context) override {
    // node_id_range
    const Tensor* node_id_range_t;
    OP_REQUIRES_OK(context, context->input("node_id_range", &node_id_range_t));
    const auto node_id_range = node_id_range_t->vec<int32>();
    const int32_t node_id_first = node_id_range(0);  // inclusive
    const int32_t node_id_last = node_id_range(1);   // exclusive

    const Tensor* stats_summary_indices_t;
    OP_REQUIRES_OK(context, context->input("stats_summary_indices",
                                           &stats_summary_indices_t));
    const auto stats_summary_indices = stats_summary_indices_t->matrix<int32>();
    const int32_t num_sparse_entries = stats_summary_indices_t->dim_size(0);

    const Tensor* stats_summary_values_t;
    OP_REQUIRES_OK(context, context->input("stats_summary_values",
                                           &stats_summary_values_t));
    const auto stats_summary_values = stats_summary_values_t->vec<float>();

    const Tensor* stats_summary_shape_t;
    OP_REQUIRES_OK(
        context, context->input("stats_summary_shape", &stats_summary_shape_t));
    const auto stats_summary_shape = stats_summary_shape_t->vec<int32>();
    const int32_t num_buckets = stats_summary_shape(2) - 1;
    const int32_t stats_dims = stats_summary_shape(3);

    const Tensor* l1_t;
    OP_REQUIRES_OK(context, context->input("l1", &l1_t));
    const auto l1 = l1_t->scalar<float>()();

    const Tensor* l2_t;
    OP_REQUIRES_OK(context, context->input("l2", &l2_t));
    const auto l2 = l2_t->scalar<float>()();

    const Tensor* tree_complexity_t;
    OP_REQUIRES_OK(context,
                   context->input("tree_complexity", &tree_complexity_t));
    const auto tree_complexity = tree_complexity_t->scalar<float>()();

    const Tensor* min_node_weight_t;
    OP_REQUIRES_OK(context,
                   context->input("min_node_weight", &min_node_weight_t));
    const auto min_node_weight = min_node_weight_t->scalar<float>()();

    std::vector<int32> output_node_ids;
    std::vector<float> output_gains;
    std::vector<int32> output_feature_dimensions;
    std::vector<int32> output_thresholds;
    std::vector<float> output_left_node_contribs;
    std::vector<float> output_right_node_contribs;
    std::vector<string> output_split_types;

    FeatureMap f_map;

    int32_t previous_node_id = -1;
    for (int idx = 0; idx < num_sparse_entries; ++idx) {
      int32_t node_id = stats_summary_indices(idx, 0);
      if (node_id != previous_node_id) {
        process_node(f_map, &output_node_ids, &output_gains,
                     &output_feature_dimensions, &output_thresholds,
                     &output_left_node_contribs, &output_right_node_contribs,
                     &output_split_types, previous_node_id, min_node_weight, l1,
                     l2, num_buckets);
        f_map.clear();
      }
      previous_node_id = node_id;
      DCHECK_LE(node_id_first, node_id);
      DCHECK_LT(node_id, node_id_last);
      const int32_t feature_dim = stats_summary_indices(idx, 1);
      const int32_t bucket_id = stats_summary_indices(idx, 2);
      const int32_t stat_dim = stats_summary_indices(idx, 3);
      std::pair<FeatureMapIterator, bool> const& f_insert_result = f_map.insert(
          FeatureMapIterator::value_type(feature_dim, BucketMap()));
      auto& b_map = f_insert_result.first->second;
      std::pair<BucketMapIterator, bool> const& b_insert_result =
          b_map.insert(BucketMapIterator::value_type(
              bucket_id, std::vector<float>(stats_dims)));
      auto& stats = b_insert_result.first->second;
      stats[stat_dim] = stats_summary_values(idx);
    }  // for node_id
    // process the last node id
    process_node(f_map, &output_node_ids, &output_gains,
                 &output_feature_dimensions, &output_thresholds,
                 &output_left_node_contribs, &output_right_node_contribs,
                 &output_split_types, previous_node_id, min_node_weight, l1, l2,
                 num_buckets);

    const int num_nodes = output_node_ids.size();
    // output_node_ids
    Tensor* output_node_ids_t = nullptr;
    OP_REQUIRES_OK(context, context->allocate_output("node_ids", {num_nodes},
                                                     &output_node_ids_t));
    auto output_node_ids_vec = output_node_ids_t->vec<int32>();

    // output_gains
    Tensor* output_gains_t;
    OP_REQUIRES_OK(context, context->allocate_output("gains", {num_nodes},
                                                     &output_gains_t));
    auto output_gains_vec = output_gains_t->vec<float>();

    // output_feature_dimensions
    Tensor* output_feature_dimension_t;
    OP_REQUIRES_OK(context,
                   context->allocate_output("feature_dimensions", {num_nodes},
                                            &output_feature_dimension_t));
    auto output_feature_dimensions_vec =
        output_feature_dimension_t->vec<int32>();

    // output_thresholds
    Tensor* output_thresholds_t;
    OP_REQUIRES_OK(context, context->allocate_output("thresholds", {num_nodes},
                                                     &output_thresholds_t));
    auto output_thresholds_vec = output_thresholds_t->vec<int32>();

    // output_left_node_contribs
    Tensor* output_left_node_contribs_t;
    OP_REQUIRES_OK(
        context, context->allocate_output("left_node_contribs", {num_nodes, 1},
                                          &output_left_node_contribs_t));
    auto output_left_node_contribs_matrix =
        output_left_node_contribs_t->matrix<float>();

    // output_right_node_contribs
    Tensor* output_right_node_contribs_t;
    OP_REQUIRES_OK(
        context, context->allocate_output("right_node_contribs", {num_nodes, 1},
                                          &output_right_node_contribs_t));
    auto output_right_node_contribs_matrix =
        output_right_node_contribs_t->matrix<float>();

    // split type
    Tensor* output_split_types_t;
    OP_REQUIRES_OK(
        context, context->allocate_output("split_with_default_directions",
                                          {num_nodes}, &output_split_types_t));
    auto output_split_types_vec = output_split_types_t->vec<tstring>();

    // Sets output tensors from vectors.
    for (int i = 0; i < num_nodes; ++i) {
      output_node_ids_vec(i) = output_node_ids[i];
      // Adjust the gains to penalize by tree complexity.
      output_gains_vec(i) = output_gains[i] - tree_complexity;
      output_feature_dimensions_vec(i) = output_feature_dimensions[i];
      output_thresholds_vec(i) = output_thresholds[i];
      // TODO(crawles): change this for multi-class.
      output_left_node_contribs_matrix(i, 0) = output_left_node_contribs[i];
      output_right_node_contribs_matrix(i, 0) = output_right_node_contribs[i];
      output_split_types_vec(i) = output_split_types[i];
    }
  }