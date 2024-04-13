PPVarArrayFromNPVariantArray::PPVarArrayFromNPVariantArray(
    PluginInstance* instance,
    size_t size,
    const NPVariant* variants)
    : size_(size) {
  if (size_ > 0) {
    array_.reset(new PP_Var[size_]);
    for (size_t i = 0; i < size_; i++)
      array_[i] = Var::NPVariantToPPVar(instance, &variants[i]);
  }
}
