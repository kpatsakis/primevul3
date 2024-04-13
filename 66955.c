JsVar *jsvMathsOp(JsVar *a, JsVar *b, int op) {
  if (op == LEX_TYPEEQUAL || op == LEX_NTYPEEQUAL) {
    bool eql = jsvMathsOpTypeEqual(a,b);
    if (op == LEX_TYPEEQUAL)
      return jsvNewFromBool(eql);
    else
      return jsvNewFromBool(!eql);
  }

  bool needsInt = op=='&' || op=='|' || op=='^' || op==LEX_LSHIFT || op==LEX_RSHIFT || op==LEX_RSHIFTUNSIGNED;
  bool needsNumeric = needsInt || op=='*' || op=='/' || op=='%' || op=='-';
  bool isCompare = op==LEX_EQUAL || op==LEX_NEQUAL || op=='<' || op==LEX_LEQUAL || op=='>'|| op==LEX_GEQUAL;
  if (isCompare) {
    if (jsvIsNumeric(a) && jsvIsString(b)) {
      needsNumeric = true;
      needsInt = jsvIsIntegerish(a) && jsvIsStringNumericInt(b, false);
    } else if (jsvIsNumeric(b) && jsvIsString(a)) {
      needsNumeric = true;
      needsInt = jsvIsIntegerish(b) && jsvIsStringNumericInt(a, false);
    }
  }

  if (jsvIsUndefined(a) && jsvIsUndefined(b)) {
    if (op == LEX_EQUAL)
      return jsvNewFromBool(true);
    else if (op == LEX_NEQUAL)
      return jsvNewFromBool(false);
    else
      return 0; // undefined
  } else if (needsNumeric ||
      ((jsvIsNumeric(a) || jsvIsUndefined(a) || jsvIsNull(a)) &&
          (jsvIsNumeric(b) || jsvIsUndefined(b) || jsvIsNull(b)))) {
    if (needsInt || (jsvIsIntegerish(a) && jsvIsIntegerish(b))) {
      JsVarInt da = jsvGetInteger(a);
      JsVarInt db = jsvGetInteger(b);
      switch (op) {
      case '+': return jsvNewFromLongInteger((long long)da + (long long)db);
      case '-': return jsvNewFromLongInteger((long long)da - (long long)db);
      case '*': return jsvNewFromLongInteger((long long)da * (long long)db);
      case '/': return jsvNewFromFloat((JsVarFloat)da/(JsVarFloat)db);
      case '&': return jsvNewFromInteger(da&db);
      case '|': return jsvNewFromInteger(da|db);
      case '^': return jsvNewFromInteger(da^db);
      case '%': return db ? jsvNewFromInteger(da%db) : jsvNewFromFloat(NAN);
      case LEX_LSHIFT: return jsvNewFromInteger(da << db);
      case LEX_RSHIFT: return jsvNewFromInteger(da >> db);
      case LEX_RSHIFTUNSIGNED: return jsvNewFromInteger((JsVarInt)(((JsVarIntUnsigned)da) >> db));
      case LEX_EQUAL:     return jsvNewFromBool(da==db && jsvIsNull(a)==jsvIsNull(b));
      case LEX_NEQUAL:    return jsvNewFromBool(da!=db || jsvIsNull(a)!=jsvIsNull(b));
      case '<':           return jsvNewFromBool(da<db);
      case LEX_LEQUAL:    return jsvNewFromBool(da<=db);
      case '>':           return jsvNewFromBool(da>db);
      case LEX_GEQUAL:    return jsvNewFromBool(da>=db);
      default: return jsvMathsOpError(op, "Integer");
      }
    } else {
      JsVarFloat da = jsvGetFloat(a);
      JsVarFloat db = jsvGetFloat(b);
      switch (op) {
      case '+': return jsvNewFromFloat(da+db);
      case '-': return jsvNewFromFloat(da-db);
      case '*': return jsvNewFromFloat(da*db);
      case '/': return jsvNewFromFloat(da/db);
      case '%': return jsvNewFromFloat(jswrap_math_mod(da, db));
      case LEX_EQUAL:
      case LEX_NEQUAL:  { bool equal = da==db;
      if ((jsvIsNull(a) && jsvIsUndefined(b)) ||
          (jsvIsNull(b) && jsvIsUndefined(a))) equal = true; // JS quirk :)
      return jsvNewFromBool((op==LEX_EQUAL) ? equal : ((bool)!equal));
      }
      case '<':           return jsvNewFromBool(da<db);
      case LEX_LEQUAL:    return jsvNewFromBool(da<=db);
      case '>':           return jsvNewFromBool(da>db);
      case LEX_GEQUAL:    return jsvNewFromBool(da>=db);
      default: return jsvMathsOpError(op, "Double");
      }
    }
  } else if ((jsvIsArray(a) || jsvIsObject(a) || jsvIsFunction(a) ||
      jsvIsArray(b) || jsvIsObject(b) || jsvIsFunction(b)) &&
      jsvIsArray(a)==jsvIsArray(b) && // Fix #283 - convert to string and test if only one is an array
      (op == LEX_EQUAL || op==LEX_NEQUAL)) {
    bool equal = a==b;

    if (jsvIsNativeFunction(a) || jsvIsNativeFunction(b)) {
      equal = a && b && 
          a->varData.native.ptr == b->varData.native.ptr &&
          a->varData.native.argTypes == b->varData.native.argTypes &&
          jsvGetFirstChild(a) == jsvGetFirstChild(b);
    }

    /* Just check pointers */
    switch (op) {
    case LEX_EQUAL:  return jsvNewFromBool(equal);
    case LEX_NEQUAL: return jsvNewFromBool(!equal);
    default: return jsvMathsOpError(op, jsvIsArray(a)?"Array":"Object");
    }
  } else {
    JsVar *da = jsvAsString(a, false);
    JsVar *db = jsvAsString(b, false);
    if (!da || !db) { // out of memory
      jsvUnLock2(da, db);
      return 0;
    }
    if (op=='+') {
      JsVar *v = jsvCopy(da, false);
      if (v) // could be out of memory
        jsvAppendStringVarComplete(v, db);
      jsvUnLock2(da, db);
      return v;
    }

    int cmp = jsvCompareString(da,db,0,0,false);
    jsvUnLock2(da, db);
    switch (op) {
    case LEX_EQUAL:     return jsvNewFromBool(cmp==0);
    case LEX_NEQUAL:    return jsvNewFromBool(cmp!=0);
    case '<':           return jsvNewFromBool(cmp<0);
    case LEX_LEQUAL:    return jsvNewFromBool(cmp<=0);
    case '>':           return jsvNewFromBool(cmp>0);
    case LEX_GEQUAL:    return jsvNewFromBool(cmp>=0);
    default: return jsvMathsOpError(op, "String");
    }
  }
}
