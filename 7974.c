  static bool ShouldSwapFreeAndContract(
      const Labels& labels, const std::vector<DimensionType>& label_types) {
    // Check that ordering is according to dimension type, with the role of
    // free and contract dimensions swapped.
    gtl::InlinedVector<int, 5> remap = {0, 1, 3, 2, 4};
    for (int i = 0; i + 1 < labels.size(); ++i) {
      const int dimtype_a = remap[label_types[labels[i]]];
      const int dimtype_b = remap[label_types[labels[i + 1]]];
      if (dimtype_a > dimtype_b ||
          (dimtype_a == dimtype_b && labels[i] > labels[i + 1])) {
        return false;
      }
    }
    return true;
  }