void PromptBrowserLoginFunction::OnLoginFailure(
    const GoogleServiceAuthError& error) {
  SendResponse(false);
  Release();
}
