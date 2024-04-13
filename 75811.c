DictionaryValue* CreateLoginResult(Profile* profile) {
  DictionaryValue* dictionary = new DictionaryValue();
  std::string username = GetBrowserSignin(profile)->GetSignedInUsername();
  dictionary->SetString(kLoginKey, username);
  if (!username.empty()) {
    CommandLine* cmdline = CommandLine::ForCurrentProcess();
    TokenService* token_service = profile->GetTokenService();
    if (cmdline->HasSwitch(switches::kAppsGalleryReturnTokens) &&
        token_service->HasTokenForService(GaiaConstants::kGaiaService)) {
      dictionary->SetString(kTokenKey,
                            token_service->GetTokenForService(
                                GaiaConstants::kGaiaService));
    }
  }
  return dictionary;
}
