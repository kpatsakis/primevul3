bool Extension::LoadAppIsolation(const DictionaryValue* manifest,
                                 std::string* error) {
  if (!CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kEnableExperimentalAppManifests))
    return true;

  Value* temp = NULL;
  if (!manifest->Get(keys::kIsolation, &temp))
    return true;

  if (temp->GetType() != Value::TYPE_LIST) {
    *error = errors::kInvalidIsolation;
    return false;
  }

  ListValue* isolation_list = static_cast<ListValue*>(temp);
  for (size_t i = 0; i < isolation_list->GetSize(); ++i) {
    std::string isolation_string;
    if (!isolation_list->GetString(i, &isolation_string)) {
      *error = ExtensionErrorUtils::FormatErrorMessage(
          errors::kInvalidIsolationValue,
          base::UintToString(i));
      return false;
    }

    if (isolation_string == values::kIsolatedStorage) {
      is_storage_isolated_ = true;
    } else {
      LOG(WARNING) << "Did not recognize isolation type: "
                   << isolation_string;
    }
  }
  return true;
}
