bool TabsUpdateFunction::UpdateURLIfPresent(DictionaryValue* update_props,
                                           int tab_id,
                                           bool* is_async) {
  if (!update_props->HasKey(keys::kUrlKey))
    return true;

  std::string url_string;
  EXTENSION_FUNCTION_VALIDATE(update_props->GetString(
      keys::kUrlKey, &url_string));
  GURL url = ExtensionTabUtil::ResolvePossiblyRelativeURL(
      url_string, GetExtension());

  if (!url.is_valid()) {
    error_ = ErrorUtils::FormatErrorMessage(
        keys::kInvalidUrlError, url_string);
    return false;
  }

  if (ExtensionTabUtil::IsCrashURL(url)) {
    error_ = keys::kNoCrashBrowserError;
    return false;
  }

  if (url.SchemeIs(chrome::kJavaScriptScheme)) {
    if (!GetExtension()->CanExecuteScriptOnPage(
            web_contents_->GetURL(),
            web_contents_->GetURL(),
            tab_id,
            NULL,
            &error_)) {
      return false;
    }

    extensions::TabHelper::FromWebContents(web_contents_)->
        script_executor()->ExecuteScript(
            extension_id(),
            ScriptExecutor::JAVASCRIPT,
            url.path(),
            ScriptExecutor::TOP_FRAME,
            extensions::UserScript::DOCUMENT_IDLE,
            ScriptExecutor::MAIN_WORLD,
            false /* is_web_view */,
            base::Bind(&TabsUpdateFunction::OnExecuteCodeFinished, this));

    *is_async = true;
    return true;
  }

  web_contents_->GetController().LoadURL(
      url, content::Referrer(), content::PAGE_TRANSITION_LINK, std::string());

  if (!url.SchemeIs(chrome::kJavaScriptScheme))
    DCHECK_EQ(url.spec(), web_contents_->GetURL().spec());

  return true;
}
