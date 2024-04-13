  static DimensionType GetDimensionType(bool is_removed, bool is_unique) {
    if (!is_removed && !is_unique)
      return kBatch;
    else if (!is_removed && is_unique)
      return kFree;
    else if (is_removed && !is_unique)
      return kContract;
    else  // is_removed && is_unique
      return kReduce;
  }