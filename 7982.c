  static bool ShouldTranspose(const TensorShape& input_shape,
                              const std::vector<int>& permutation) {
    if (input_shape.dims() < 2) return false;
    for (int i = 0; i < permutation.size(); ++i) {
      if (permutation[i] != i) return true;
    }
    return false;
  }