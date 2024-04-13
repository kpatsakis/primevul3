JsVar *jsvCopy(JsVar *src, bool copyChildren) {
  if (jsvIsFlatString(src)) {
    return jsvNewFromStringVar(src, 0, JSVAPPENDSTRINGVAR_MAXLENGTH);
  }
  JsVar *dst = jsvNewWithFlags(src->flags & JSV_VARIABLEINFOMASK);
  if (!dst) return 0; // out of memory
  if (!jsvIsStringExt(src)) {
      memcpy(&dst->varData, &src->varData, (jsvIsBasicString(src)||jsvIsNativeString(src)) ? JSVAR_DATA_STRING_LEN : JSVAR_DATA_STRING_NAME_LEN);
      if (!(jsvIsBasicString(src)||jsvIsNativeString(src))) {
        assert(jsvGetPrevSibling(dst) == 0);
        assert(jsvGetNextSibling(dst) == 0);
        assert(jsvGetFirstChild(dst) == 0);
      }
      assert(jsvGetLastChild(dst) == 0);
  } else {
    memcpy(&dst->varData, &src->varData, JSVAR_DATA_STRING_MAX_LEN);
    assert(jsvGetLastChild(dst) == 0);
  }

  if (copyChildren && jsvIsName(src)) {
    if (jsvGetFirstChild(src)) {
      if (jsvIsNameWithValue(src)) {
        jsvSetFirstChild(dst, jsvGetFirstChild(src));
      } else {
        JsVar *child = jsvLock(jsvGetFirstChild(src));
        JsVar *childCopy = jsvRef(jsvCopy(child, true));
        jsvUnLock(child);
        if (childCopy) { // could have been out of memory
          jsvSetFirstChild(dst, jsvGetRef(childCopy));
          jsvUnLock(childCopy);
        }
      }
    }
  }

  if (jsvHasStringExt(src)) {
    if (jsvGetLastChild(src)) {
      JsVar *child = jsvLock(jsvGetLastChild(src));
      JsVar *childCopy = jsvCopy(child, true);
      if (childCopy) {// could be out of memory
        jsvSetLastChild(dst, jsvGetRef(childCopy)); // no ref for stringext
        jsvUnLock(childCopy);
      }
      jsvUnLock(child);
    }
  } else if (jsvHasChildren(src)) {
    if (copyChildren) {
      JsVarRef vr;
      vr = jsvGetFirstChild(src);
      while (vr) {
        JsVar *name = jsvLock(vr);
        JsVar *child = jsvCopyNameOnly(name, true/*link children*/, true/*keep as name*/); // NO DEEP COPY!
        if (child) { // could have been out of memory
          jsvAddName(dst, child);
          jsvUnLock(child);
        }
        vr = jsvGetNextSibling(name);
        jsvUnLock(name);
      }
    }
  } else {
    assert(jsvIsBasic(src)); // in case we missed something!
  }

  return dst;
}
