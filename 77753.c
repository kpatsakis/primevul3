bool AreWeShowingSignin(GURL url, signin::Source source, std::string email) {
  GURL::Replacements replacements;
  replacements.ClearQuery();
  GURL clean_login_url =
      GaiaUrls::GetInstance()->service_login_url().ReplaceComponents(
          replacements);

  return (url.ReplaceComponents(replacements) == clean_login_url &&
          source != signin::SOURCE_UNKNOWN) ||
      (IsValidGaiaSigninRedirectOrResponseURL(url) &&
       url.spec().find("ChromeLoginPrompt") != std::string::npos &&
       !email.empty());
}
