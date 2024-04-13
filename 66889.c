size_t jsvGetMaxCharactersInVar(const JsVar *v) {
  if (jsvIsStringExt(v)) return JSVAR_DATA_STRING_MAX_LEN;
  assert(jsvHasCharacterData(v));
  if (jsvIsName(v)) return JSVAR_DATA_STRING_NAME_LEN;
  return JSVAR_DATA_STRING_LEN;
}
