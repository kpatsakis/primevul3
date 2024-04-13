bool WindowsCreateFunction::ShouldOpenIncognitoWindow(
    const base::DictionaryValue* args,
    std::vector<GURL>* urls,
    bool* is_error) {
  *is_error = false;
  const IncognitoModePrefs::Availability incognito_availability =
      IncognitoModePrefs::GetAvailability(profile_->GetPrefs());
  bool incognito = false;
  if (args && args->HasKey(keys::kIncognitoKey)) {
    EXTENSION_FUNCTION_VALIDATE(args->GetBoolean(keys::kIncognitoKey,
                                                 &incognito));
    if (incognito && incognito_availability == IncognitoModePrefs::DISABLED) {
      error_ = keys::kIncognitoModeIsDisabled;
      *is_error = true;
      return false;
    }
    if (!incognito && incognito_availability == IncognitoModePrefs::FORCED) {
      error_ = keys::kIncognitoModeIsForced;
      *is_error = true;
      return false;
    }
  } else if (incognito_availability == IncognitoModePrefs::FORCED) {
    incognito = true;
  }

  if (incognito && !profile_->IsGuestSession()) {
    std::string first_url_erased;
    for (size_t i = 0; i < urls->size();) {
      if (chrome::IsURLAllowedInIncognito((*urls)[i], profile())) {
        i++;
      } else {
        if (first_url_erased.empty())
          first_url_erased = (*urls)[i].spec();
        urls->erase(urls->begin() + i);
      }
    }
    if (urls->empty() && !first_url_erased.empty()) {
      error_ = ErrorUtils::FormatErrorMessage(
          keys::kURLsNotAllowedInIncognitoError, first_url_erased);
      *is_error = true;
      return false;
    }
  }
  return incognito;
}
