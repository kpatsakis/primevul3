bool PromptBrowserLoginFunction::RunImpl() {
  if (!IsWebStoreURL(profile_, source_url()))
    return false;

  std::string preferred_email;
  if (args_->GetSize() > 0) {
    EXTENSION_FUNCTION_VALIDATE(args_->GetString(0, &preferred_email));
  }

  Profile* profile = GetDefaultProfile(profile_);

  TabContents* tab = dispatcher()->delegate()->associated_tab_contents();
  if (!tab)
    return false;

  AddRef();

  TokenService* token_service = profile->GetTokenService();
  registrar_.Add(this,
                 NotificationType::TOKEN_AVAILABLE,
                 Source<TokenService>(token_service));
  registrar_.Add(this,
                 NotificationType::TOKEN_REQUEST_FAILED,
                 Source<TokenService>(token_service));

  GetBrowserSignin(profile)->RequestSignin(tab,
                                           ASCIIToUTF16(preferred_email),
                                           GetLoginMessage(),
                                           this);

  return true;
}
