bool PPResultAndExceptionToNPResult::CheckExceptionForNoResult() {
  DCHECK(!checked_exception_);  // Don't call more than once.
  DCHECK(!np_result_);  // Can't have a result when doing this.

  checked_exception_ = true;

  if (has_exception()) {
    ThrowException();
    success_ = false;
    return false;
  }
  success_ = true;
  return true;
}
