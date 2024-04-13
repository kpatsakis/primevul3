const ExampleStatistics Example::ComputeWxAndWeightedExampleNorm(
    const int num_loss_partitions, const ModelWeights& model_weights,
    const Regularizations& regularization, const int num_weight_vectors) const {
  ExampleStatistics result(num_weight_vectors);

  result.normalized_squared_norm =
      squared_norm_ / regularization.symmetric_l2();

  // Compute w \dot x and prev_w \dot x.
  // This is for sparse features contribution to the logit.
  for (size_t j = 0; j < sparse_features_.size(); ++j) {
    const Example::SparseFeatures& sparse_features = sparse_features_[j];
    const FeatureWeightsSparseStorage& sparse_weights =
        model_weights.sparse_weights()[j];

    for (int64_t k = 0; k < sparse_features.indices->size(); ++k) {
      const int64_t feature_index = (*sparse_features.indices)(k);
      const double feature_value = sparse_features.values == nullptr
                                       ? 1.0
                                       : (*sparse_features.values)(k);
      for (int l = 0; l < num_weight_vectors; ++l) {
        const float sparse_weight = sparse_weights.nominals(l, feature_index);
        const double feature_weight =
            sparse_weight +
            sparse_weights.deltas(l, feature_index) * num_loss_partitions;
        result.prev_wx[l] +=
            feature_value * regularization.Shrink(sparse_weight);
        result.wx[l] += feature_value * regularization.Shrink(feature_weight);
      }
    }
  }

  // Compute w \dot x and prev_w \dot x.
  // This is for dense features contribution to the logit.
  for (size_t j = 0; j < dense_vectors_.size(); ++j) {
    const Example::DenseVector& dense_vector = *dense_vectors_[j];
    const FeatureWeightsDenseStorage& dense_weights =
        model_weights.dense_weights()[j];

    const Eigen::Tensor<float, 2, Eigen::RowMajor> feature_weights =
        dense_weights.nominals() +
        dense_weights.deltas() *
            dense_weights.deltas().constant(num_loss_partitions);
    if (num_weight_vectors == 1) {
      const Eigen::Tensor<float, 0, Eigen::RowMajor> prev_prediction =
          (dense_vector.Row() *
           regularization.EigenShrinkVector(
               Eigen::TensorMap<Eigen::Tensor<const float, 1, Eigen::RowMajor>>(
                   dense_weights.nominals().data(),
                   dense_weights.nominals().dimension(1))))
              .sum();
      const Eigen::Tensor<float, 0, Eigen::RowMajor> prediction =
          (dense_vector.Row() *
           regularization.EigenShrinkVector(
               Eigen::TensorMap<Eigen::Tensor<const float, 1, Eigen::RowMajor>>(
                   feature_weights.data(), feature_weights.dimension(1))))
              .sum();
      result.prev_wx[0] += prev_prediction();
      result.wx[0] += prediction();
    } else {
      const Eigen::array<Eigen::IndexPair<int>, 1> product_dims = {
          Eigen::IndexPair<int>(1, 1)};
      const Eigen::Tensor<float, 2, Eigen::RowMajor> prev_prediction =
          regularization.EigenShrinkMatrix(dense_weights.nominals())
              .contract(dense_vector.RowAsMatrix(), product_dims);
      const Eigen::Tensor<float, 2, Eigen::RowMajor> prediction =
          regularization.EigenShrinkMatrix(feature_weights)
              .contract(dense_vector.RowAsMatrix(), product_dims);
      // The result of "tensor contraction" (multiplication)  in the code
      // above is of dimension num_weight_vectors * 1.
      for (int l = 0; l < num_weight_vectors; ++l) {
        result.prev_wx[l] += prev_prediction(l, 0);
        result.wx[l] += prediction(l, 0);
      }
    }
  }

  return result;
}