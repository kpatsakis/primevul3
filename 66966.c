NO_INLINE void jspeBlockNoBrackets() {
  if (JSP_SHOULD_EXECUTE) {
    while (lex->tk && lex->tk!='}') {
      jsvUnLock(jspeStatement());
      if (JSP_HAS_ERROR) {
        if (lex && !(execInfo.execute&EXEC_ERROR_LINE_REPORTED)) {
          execInfo.execute = (JsExecFlags)(execInfo.execute | EXEC_ERROR_LINE_REPORTED);
          JsVar *stackTrace = jsvObjectGetChild(execInfo.hiddenRoot, JSPARSE_STACKTRACE_VAR, JSV_STRING_0);
          if (stackTrace) {
            jsvAppendPrintf(stackTrace, "at ");
            jspAppendStackTrace(stackTrace);
            jsvUnLock(stackTrace);
          }
        }
      }
      if (JSP_SHOULDNT_PARSE)
        return;
    }
  } else {
    int brackets = 0;
    while (lex->tk && (brackets || lex->tk != '}')) {
      if (lex->tk == '{') brackets++;
      if (lex->tk == '}') brackets--;
      JSP_ASSERT_MATCH(lex->tk);
    }
