Status Examples::CreateDenseFeatureRepresentation(
    const DeviceBase::CpuWorkerThreads& worker_threads, const int num_examples,
    const int num_dense_features, const ModelWeights& weights,
    const OpInputList& dense_features_inputs,
    std::vector<Example>* const examples) {
  mutex mu;
  Status result;  // Guarded by mu
  auto parse_partition = [&](const int64_t begin, const int64_t end) {
    // The static_cast here is safe since begin and end can be at most
    // num_examples which is an int.
    for (int i = static_cast<int>(begin); i < end; ++i) {
      auto dense_features = dense_features_inputs[i].template matrix<float>();
      for (int example_id = 0; example_id < num_examples; ++example_id) {
        (*examples)[example_id].dense_vectors_[i].reset(
            new Example::DenseVector{dense_features, example_id});
      }
      if (!weights.DenseIndexValid(i, dense_features.dimension(1) - 1)) {
        mutex_lock l(mu);
        result = errors::InvalidArgument(
            "More dense features than we have parameters for: ",
            dense_features.dimension(1));
        return;
      }
    }
  };
  // TODO(sibyl-Aix6ihai): Tune this as a function of dataset size.
  const int64_t kCostPerUnit = num_examples;
  Shard(worker_threads.num_threads, worker_threads.workers, num_dense_features,
        kCostPerUnit, parse_partition);
  return result;
}