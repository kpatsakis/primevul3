PPResultAndExceptionToNPResult::PPResultAndExceptionToNPResult(
    NPObject* object_var,
    NPVariant* np_result)
    : object_var_(object_var),
      np_result_(np_result),
      exception_(PP_MakeUndefined()),
      success_(false),
      checked_exception_(false) {
}
