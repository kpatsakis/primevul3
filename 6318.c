void FeatureWeightsDenseStorage::UpdateDenseDeltaWeights(
    const Eigen::ThreadPoolDevice& device,
    const Example::DenseVector& dense_vector,
    const std::vector<double>& normalized_bounded_dual_delta) {
  const size_t num_weight_vectors = normalized_bounded_dual_delta.size();
  if (num_weight_vectors == 1) {
    deltas_.device(device) =
        deltas_ + dense_vector.RowAsMatrix() *
                      deltas_.constant(normalized_bounded_dual_delta[0]);
  } else {
    // Transform the dual vector into a column matrix.
    const Eigen::TensorMap<Eigen::Tensor<const double, 2, Eigen::RowMajor>>
        dual_matrix(normalized_bounded_dual_delta.data(), num_weight_vectors,
                    1);
    const Eigen::array<Eigen::IndexPair<int>, 1> product_dims = {
        Eigen::IndexPair<int>(1, 0)};
    // This computes delta_w += delta_vector / \lamdba * N.
    deltas_.device(device) =
        (deltas_.cast<double>() +
         dual_matrix.contract(dense_vector.RowAsMatrix().cast<double>(),
                              product_dims))
            .cast<float>();
  }
}