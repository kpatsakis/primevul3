NPObjectAccessorWithIdentifier::NPObjectAccessorWithIdentifier(
    NPObject* object,
    NPIdentifier identifier,
    bool allow_integer_identifier)
    : object_(PluginObject::FromNPObject(object)),
      identifier_(PP_MakeUndefined()) {
  if (object_) {
    identifier_ = Var::NPIdentifierToPPVar(object_->instance()->module(),
                                           identifier);
    if (identifier_.type == PP_VARTYPE_INT32 && !allow_integer_identifier)
      identifier_.type = PP_VARTYPE_UNDEFINED;  // Mark it invalid.
  }
}
