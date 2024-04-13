void PPResultAndExceptionToNPResult::ThrowException() {
  scoped_refptr<StringVar> string(StringVar::FromPPVar(exception_));
  if (string) {
    WebBindings::setException(object_var_, string->value().c_str());
  }
}
