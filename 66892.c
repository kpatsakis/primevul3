void *jsvGetNativeFunctionPtr(const JsVar *function) {
  /* see descriptions in jsvar.h. If we have a child called JSPARSE_FUNCTION_CODE_NAME
   * then we execute code straight from that */
  JsVar *flatString = jsvFindChildFromString((JsVar*)function, JSPARSE_FUNCTION_CODE_NAME, 0);
  if (flatString) {
    flatString = jsvSkipNameAndUnLock(flatString);
    void *v = (void*)((size_t)function->varData.native.ptr + (char*)jsvGetFlatStringPointer(flatString));
    jsvUnLock(flatString);
    return v;
  } else
    return (void *)function->varData.native.ptr;
}
