PPResultAndExceptionToNPResult::~PPResultAndExceptionToNPResult() {
  DCHECK(checked_exception_);

  ObjectVar::PluginReleasePPVar(exception_);
}
