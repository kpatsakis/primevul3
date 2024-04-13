  static void PermuteLabels(const std::vector<int>& permutation,
                            Labels* labels) {
    Labels permuted_labels(labels->size());
    for (int i = 0; i < labels->size(); ++i) {
      permuted_labels[i] = (*labels)[permutation[i]];
    }
    labels->swap(permuted_labels);
  }