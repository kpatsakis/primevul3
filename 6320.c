void ModelWeights::UpdateDeltaWeights(
    const Eigen::ThreadPoolDevice& device, const Example& example,
    const std::vector<double>& normalized_bounded_dual_delta) {
  // Sparse weights.
  for (size_t j = 0; j < sparse_weights_.size(); ++j) {
    sparse_weights_[j].UpdateSparseDeltaWeights(
        device, example.sparse_features_[j], normalized_bounded_dual_delta);
  }

  // Dense weights.
  for (size_t j = 0; j < dense_weights_.size(); ++j) {
    dense_weights_[j].UpdateDenseDeltaWeights(
        device, *example.dense_vectors_[j], normalized_bounded_dual_delta);
  }
}