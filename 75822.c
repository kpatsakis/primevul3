  virtual void OnJSONParseSucceeded(const ListValue& wrapper) {
    CHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));
    manifest_parse_complete_ = true;
    Value* value = NULL;
    CHECK(wrapper.Get(0, &value));
    if (value->IsType(Value::TYPE_DICTIONARY)) {
      parsed_manifest_.reset(
          static_cast<DictionaryValue*>(value)->DeepCopy());
    } else {
      parse_error_ = BeginInstallWithManifestFunction::MANIFEST_ERROR;
    }
    ReportResultsIfComplete();
  }
