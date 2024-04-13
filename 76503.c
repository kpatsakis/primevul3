void LoginUtilsImpl::CompleteLogin(
    const std::string& username,
    const std::string& password,
    const GaiaAuthConsumer::ClientLoginResult& credentials) {
  BootTimesLoader* btl = BootTimesLoader::Get();

  VLOG(1) << "Completing login for " << username;
  btl->AddLoginTimeMarker("CompletingLogin", false);

  if (CrosLibrary::Get()->EnsureLoaded()) {
    CrosLibrary::Get()->GetLoginLibrary()->StartSession(username, "");
    btl->AddLoginTimeMarker("StartedSession", false);
  }

  bool first_login = !UserManager::Get()->IsKnownUser(username);
  UserManager::Get()->UserLoggedIn(username);
  btl->AddLoginTimeMarker("UserLoggedIn", false);

  FilePath user_data_dir;
  PathService::Get(chrome::DIR_USER_DATA, &user_data_dir);
  ProfileManager* profile_manager = g_browser_process->profile_manager();

  logging::RedirectChromeLogging(*(CommandLine::ForCurrentProcess()));
  btl->AddLoginTimeMarker("LoggingRedirected", false);

  Profile* profile = profile_manager->GetDefaultProfile(user_data_dir);
  btl->AddLoginTimeMarker("UserProfileGotten", false);

  CookieFetcher* cf = new CookieFetcher(profile);
  cf->AttemptFetch(credentials.data);
  btl->AddLoginTimeMarker("CookieFetchStarted", false);

  if (profile->GetExtensionsService() &&
      profile->GetExtensionsService()->extensions_enabled()) {
    profile->GetExtensionsService()->InitEventRouters();
  }
  btl->AddLoginTimeMarker("ExtensionsServiceStarted", false);

  TokenService* token_service = profile->GetTokenService();
  token_service->Initialize(GaiaConstants::kChromeOSSource,
                            profile);
  token_service->LoadTokensFromDB();
  token_service->UpdateCredentials(credentials);
  if (token_service->AreCredentialsValid()) {
    token_service->StartFetchingTokens();
  }
  btl->AddLoginTimeMarker("TokensGotten", false);

  profile->GetProfileSyncService(username)->SetPassphrase(password);
  btl->AddLoginTimeMarker("SyncStarted", false);

  OwnershipService::GetSharedInstance()->StartTakeOwnershipAttempt(
      UserManager::Get()->logged_in_user().email());
  if (CrosLibrary::Get()->EnsureLoaded()) {
    CryptohomeLibrary* cryptohome = CrosLibrary::Get()->GetCryptohomeLibrary();
    if (cryptohome->TpmIsEnabled() && !cryptohome->TpmIsBeingOwned()) {
      if (cryptohome->TpmIsOwned()) {
        cryptohome->TpmClearStoredPassword();
      } else {
        cryptohome->TpmCanAttemptOwnership();
      }
    }
  }
  btl->AddLoginTimeMarker("TPMOwned", false);

  static const char kFallbackInputMethodLocale[] = "en-US";
  if (first_login) {
    std::string locale(g_browser_process->GetApplicationLocale());
    if (locale != kFallbackInputMethodLocale) {
      StringPrefMember language_preload_engines;
      language_preload_engines.Init(prefs::kLanguagePreloadEngines,
                                    profile->GetPrefs(), NULL);
      StringPrefMember language_preferred_languages;
      language_preferred_languages.Init(prefs::kLanguagePreferredLanguages,
                                        profile->GetPrefs(), NULL);

      std::string preload_engines(language_preload_engines.GetValue());
      std::vector<std::string> input_method_ids;
      input_method::GetInputMethodIdsFromLanguageCode(
          locale, input_method::kAllInputMethods, &input_method_ids);
      if (!input_method_ids.empty()) {
        if (!preload_engines.empty())
          preload_engines += ',';
        preload_engines += input_method_ids[0];
      }
      language_preload_engines.SetValue(preload_engines);

      std::string preferred_languages(locale);
      preferred_languages += ",";
      preferred_languages += kFallbackInputMethodLocale;
      language_preferred_languages.SetValue(preferred_languages);
      btl->AddLoginTimeMarker("IMESTarted", false);
    }
  }

  if (EndsWith(username, "@google.com", true)) {
    PrefService* pref_service = profile->GetPrefs();
    pref_service->SetBoolean(prefs::kEnableScreenLock, true);
  }

  DoBrowserLaunch(profile);
}
