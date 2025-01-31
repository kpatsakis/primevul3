Status Examples::CreateSparseFeatureRepresentation(
    const DeviceBase::CpuWorkerThreads& worker_threads, const int num_examples,
    const int num_sparse_features, const ModelWeights& weights,
    const OpInputList& sparse_example_indices_inputs,
    const OpInputList& sparse_feature_indices_inputs,
    const OpInputList& sparse_feature_values_inputs,
    std::vector<Example>* const examples) {
  mutex mu;
  Status result;  // Guarded by mu
  auto parse_partition = [&](const int64_t begin, const int64_t end) {
    // The static_cast here is safe since begin and end can be at most
    // num_examples which is an int.
    for (int i = static_cast<int>(begin); i < end; ++i) {
      auto example_indices =
          sparse_example_indices_inputs[i].template flat<int64>();
      auto feature_indices =
          sparse_feature_indices_inputs[i].template flat<int64>();
      if (example_indices.size() != feature_indices.size()) {
        mutex_lock l(mu);
        result = errors::InvalidArgument(
            "Found mismatched example_indices and feature_indices [",
            example_indices, "] vs [", feature_indices, "]");
        return;
      }

      // Parse features for each example. Features for a particular example
      // are at the offsets (start_id, end_id]
      int start_id = -1;
      int end_id = 0;
      for (int example_id = 0; example_id < num_examples; ++example_id) {
        start_id = end_id;
        while (end_id < example_indices.size() &&
               example_indices(end_id) == example_id) {
          ++end_id;
        }
        Example::SparseFeatures* const sparse_features =
            &(*examples)[example_id].sparse_features_[i];
        if (start_id < example_indices.size() &&
            example_indices(start_id) == example_id) {
          sparse_features->indices.reset(new UnalignedInt64Vector(
              &(feature_indices(start_id)), end_id - start_id));
          if (sparse_feature_values_inputs.size() > i) {
            auto feature_weights =
                sparse_feature_values_inputs[i].flat<float>();
            sparse_features->values.reset(new UnalignedFloatVector(
                &(feature_weights(start_id)), end_id - start_id));
          }
          // If features are non empty.
          if (end_id - start_id > 0) {
            // TODO(sibyl-Aix6ihai): Write this efficiently using vectorized
            // operations from eigen.
            for (int64_t k = 0; k < sparse_features->indices->size(); ++k) {
              const int64_t feature_index = (*sparse_features->indices)(k);
              if (!weights.SparseIndexValid(i, feature_index)) {
                mutex_lock l(mu);
                result = errors::InvalidArgument(
                    "Found sparse feature indices out of valid range: ",
                    (*sparse_features->indices)(k));
                return;
              }
            }
          }
        } else {
          // Add a Tensor that has size 0.
          sparse_features->indices.reset(
              new UnalignedInt64Vector(&(feature_indices(0)), 0));
          // If values exist for this feature group.
          if (sparse_feature_values_inputs.size() > i) {
            auto feature_weights =
                sparse_feature_values_inputs[i].flat<float>();
            sparse_features->values.reset(
                new UnalignedFloatVector(&(feature_weights(0)), 0));
          }
        }
      }
    }
  };
  // For each column, the cost of parsing it is O(num_examples). We use
  // num_examples here, as empirically Shard() creates the right amount of
  // threads based on the problem size.
  // TODO(sibyl-Aix6ihai): Tune this as a function of dataset size.
  const int64_t kCostPerUnit = num_examples;
  Shard(worker_threads.num_threads, worker_threads.workers, num_sparse_features,
        kCostPerUnit, parse_partition);
  return result;
}