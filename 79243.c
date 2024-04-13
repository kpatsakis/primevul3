QueryArg ParseBoolQueryArg(base::DictionaryValue* query, const char* key) {
  if (query->HasKey(key)) {
    bool value = false;
    CHECK(query->GetBoolean(key, &value));
    return value ? MATCH_TRUE : MATCH_FALSE;
  }
  return NOT_SET;
}
