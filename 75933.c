NPObjectAccessorWithIdentifier::~NPObjectAccessorWithIdentifier() {
  Var::PluginReleasePPVar(identifier_);
}
