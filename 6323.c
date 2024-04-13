void FeatureWeightsSparseStorage::UpdateSparseDeltaWeights(
    const Eigen::ThreadPoolDevice& device,
    const Example::SparseFeatures& sparse_features,
    const std::vector<double>& normalized_bounded_dual_delta) {
  for (int64_t k = 0; k < sparse_features.indices->size(); ++k) {
    const double feature_value =
        sparse_features.values == nullptr ? 1.0 : (*sparse_features.values)(k);
    auto it = indices_to_id_.find((*sparse_features.indices)(k));
    for (size_t l = 0; l < normalized_bounded_dual_delta.size(); ++l) {
      deltas_(l, it->second) +=
          feature_value * normalized_bounded_dual_delta[l];
    }
  }
}