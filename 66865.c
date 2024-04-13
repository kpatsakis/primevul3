size_t jsvGetCharactersInVar(const JsVar *v) {
  unsigned int f = v->flags&JSV_VARTYPEMASK;
  if (f == JSV_FLAT_STRING)
    return (size_t)v->varData.integer;
  if (f == JSV_NATIVE_STRING)
    return (size_t)v->varData.nativeStr.len;
  assert(f >= JSV_NAME_STRING_INT_0);
  assert((JSV_NAME_STRING_INT_0 < JSV_NAME_STRING_0) &&
         (JSV_NAME_STRING_0 < JSV_STRING_0) &&
         (JSV_STRING_0 < JSV_STRING_EXT_0)); // this relies on ordering
  if (f<=JSV_NAME_STRING_MAX) {
    if (f<=JSV_NAME_STRING_INT_MAX)
      return f-JSV_NAME_STRING_INT_0;
    else
      return f-JSV_NAME_STRING_0;
  } else {
    if (f<=JSV_STRING_MAX) return f-JSV_STRING_0;
    assert(f <= JSV_STRING_EXT_MAX);
    return f - JSV_STRING_EXT_0;
  }
}
