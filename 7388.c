bool FindZFPCompressionParam(ZFPCompressionParam *param,
                             const EXRAttribute *attributes,
                             int num_attributes) {
  bool foundType = false;

  for (int i = 0; i < num_attributes; i++) {
    if ((strcmp(attributes[i].name, "zfpCompressionType") == 0) &&
        (attributes[i].size == 1)) {
      param->type = static_cast<int>(attributes[i].value[0]);

      foundType = true;
    }
  }

  if (!foundType) {
    return false;
  }

  if (param->type == TINYEXR_ZFP_COMPRESSIONTYPE_RATE) {
    for (int i = 0; i < num_attributes; i++) {
      if ((strcmp(attributes[i].name, "zfpCompressionRate") == 0) &&
          (attributes[i].size == 8)) {
        param->rate = *(reinterpret_cast<double *>(attributes[i].value));
        return true;
      }
    }
  } else if (param->type == TINYEXR_ZFP_COMPRESSIONTYPE_PRECISION) {
    for (int i = 0; i < num_attributes; i++) {
      if ((strcmp(attributes[i].name, "zfpCompressionPrecision") == 0) &&
          (attributes[i].size == 4)) {
        param->rate = *(reinterpret_cast<int *>(attributes[i].value));
        return true;
      }
    }
  } else if (param->type == TINYEXR_ZFP_COMPRESSIONTYPE_ACCURACY) {
    for (int i = 0; i < num_attributes; i++) {
      if ((strcmp(attributes[i].name, "zfpCompressionTolerance") == 0) &&
          (attributes[i].size == 8)) {
        param->tolerance = *(reinterpret_cast<double *>(attributes[i].value));
        return true;
      }
    }
  } else {
    assert(0);
  }

  return false;
}