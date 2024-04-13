void jsvAddName(JsVar *parent, JsVar *namedChild) {
  namedChild = jsvRef(namedChild); // ref here VERY important as adding to structure!
  assert(jsvIsName(namedChild));

  if (jsvIsArray(parent) && jsvIsInt(namedChild)) {
    JsVarInt index = namedChild->varData.integer;
    if (index >= jsvGetArrayLength(parent)) {
      jsvSetArrayLength(parent, index + 1, false);
    }
  }

  if (jsvGetLastChild(parent)) { // we have children already
    JsVar *insertAfter = jsvLock(jsvGetLastChild(parent));
    if (jsvIsArray(parent)) {
      while (insertAfter && jsvCompareInteger(namedChild, insertAfter)<0) {
        JsVarRef prev = jsvGetPrevSibling(insertAfter);
        jsvUnLock(insertAfter);
        insertAfter = prev ? jsvLock(prev) : 0;
      }
    }

    if (insertAfter) {
      if (jsvGetNextSibling(insertAfter)) {
        JsVar *insertBefore = jsvLock(jsvGetNextSibling(insertAfter));
        jsvSetPrevSibling(insertBefore, jsvGetRef(namedChild));
        jsvSetNextSibling(namedChild, jsvGetRef(insertBefore));
        jsvUnLock(insertBefore);
      } else {
        jsvSetLastChild(parent, jsvGetRef(namedChild));
      }
      jsvSetNextSibling(insertAfter, jsvGetRef(namedChild));
      jsvSetPrevSibling(namedChild, jsvGetRef(insertAfter));
      jsvUnLock(insertAfter);
    } else { // Insert right at the beginning of the array
      JsVar *firstChild = jsvLock(jsvGetFirstChild(parent));
      jsvSetPrevSibling(firstChild, jsvGetRef(namedChild));
      jsvUnLock(firstChild);

      jsvSetNextSibling(namedChild, jsvGetFirstChild(parent));
      jsvSetFirstChild(parent, jsvGetRef(namedChild));
    }
  } else { // we have no children - just add it
    JsVarRef r = jsvGetRef(namedChild);
    jsvSetFirstChild(parent, r);
    jsvSetLastChild(parent, r);
  }
}
