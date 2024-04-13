PPVarArrayFromNPVariantArray::~PPVarArrayFromNPVariantArray() {
  for (size_t i = 0; i < size_; i++)
    Var::PluginReleasePPVar(array_[i]);
}
