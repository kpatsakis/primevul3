static void jsvGarbageCollectMarkUsed(JsVar *var) {
  var->flags &= (JsVarFlags)~JSV_GARBAGE_COLLECT;

  if (jsvHasCharacterData(var)) {
    JsVarRef child = jsvGetLastChild(var);
    while (child) {
      JsVar *childVar;
      childVar = jsvGetAddressOf(child);
      childVar->flags &= (JsVarFlags)~JSV_GARBAGE_COLLECT;
      child = jsvGetLastChild(childVar);
    }
  }
  if (jsvHasSingleChild(var)) {
    if (jsvGetFirstChild(var)) {
      JsVar *childVar = jsvGetAddressOf(jsvGetFirstChild(var));
      if (childVar->flags & JSV_GARBAGE_COLLECT)
        jsvGarbageCollectMarkUsed(childVar);
    }
  } else if (jsvHasChildren(var)) {
    JsVarRef child = jsvGetFirstChild(var);
    while (child) {
      JsVar *childVar;
      childVar = jsvGetAddressOf(child);
      if (childVar->flags & JSV_GARBAGE_COLLECT)
        jsvGarbageCollectMarkUsed(childVar);
      child = jsvGetNextSibling(childVar);
    }
  }
}
