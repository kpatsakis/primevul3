ChildProcessSecurityPolicyImpl::ChildProcessSecurityPolicyImpl() {
  RegisterWebSafeScheme(chrome::kHttpScheme);
  RegisterWebSafeScheme(chrome::kHttpsScheme);
  RegisterWebSafeScheme(chrome::kFtpScheme);
  RegisterWebSafeScheme(chrome::kDataScheme);
  RegisterWebSafeScheme("feed");
  RegisterWebSafeScheme(chrome::kBlobScheme);
  RegisterWebSafeScheme(chrome::kFileSystemScheme);

  RegisterPseudoScheme(chrome::kAboutScheme);
  RegisterPseudoScheme(chrome::kJavaScriptScheme);
  RegisterPseudoScheme(chrome::kViewSourceScheme);
}
