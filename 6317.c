Status Examples::ComputeSquaredNormPerExample(
    const DeviceBase::CpuWorkerThreads& worker_threads, const int num_examples,
    const int num_sparse_features, const int num_dense_features,
    std::vector<Example>* const examples) {
  mutex mu;
  Status result;  // Guarded by mu
  // Compute norm of examples.
  auto compute_example_norm = [&](const int64_t begin, const int64_t end) {
    // The static_cast here is safe since begin and end can be at most
    // num_examples which is an int.
    gtl::FlatSet<int64> previous_indices;
    for (int example_id = static_cast<int>(begin); example_id < end;
         ++example_id) {
      double squared_norm = 0;
      Example* const example = &(*examples)[example_id];
      for (int j = 0; j < num_sparse_features; ++j) {
        const Example::SparseFeatures& sparse_features =
            example->sparse_features_[j];
        previous_indices.clear();
        for (int64_t k = 0; k < sparse_features.indices->size(); ++k) {
          const int64_t feature_index = (*sparse_features.indices)(k);
          if (previous_indices.insert(feature_index).second == false) {
            mutex_lock l(mu);
            result =
                errors::InvalidArgument("Duplicate index in sparse vector.");
            return;
          }
          const double feature_value = sparse_features.values == nullptr
                                           ? 1.0
                                           : (*sparse_features.values)(k);
          squared_norm += feature_value * feature_value;
        }
      }
      for (int j = 0; j < num_dense_features; ++j) {
        const Eigen::Tensor<float, 0, Eigen::RowMajor> sn =
            example->dense_vectors_[j]->Row().square().sum();
        squared_norm += sn();
      }
      example->squared_norm_ = squared_norm;
    }
  };
  // TODO(sibyl-Aix6ihai): Compute the cost optimally.
  const int64_t kCostPerUnit = num_dense_features + num_sparse_features;
  Shard(worker_threads.num_threads, worker_threads.workers, num_examples,
        kCostPerUnit, compute_example_norm);
  return result;
}