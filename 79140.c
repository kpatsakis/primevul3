void DevToolsWindow::AddDevToolsExtensionsToClient() {
  content::WebContents* inspected_web_contents = GetInspectedWebContents();
  if (inspected_web_contents) {
    SessionTabHelper* session_tab_helper =
        SessionTabHelper::FromWebContents(inspected_web_contents);
    if (session_tab_helper) {
      base::FundamentalValue tabId(session_tab_helper->session_id().id());
      CallClientFunction("WebInspector.setInspectedTabId", &tabId, NULL, NULL);
    }
  }

  Profile* profile =
      Profile::FromBrowserContext(web_contents_->GetBrowserContext());
  const ExtensionService* extension_service = extensions::ExtensionSystem::Get(
      profile->GetOriginalProfile())->extension_service();
  if (!extension_service)
    return;
  const ExtensionSet* extensions = extension_service->extensions();

  ListValue results;
  for (ExtensionSet::const_iterator extension(extensions->begin());
       extension != extensions->end(); ++extension) {
    if (extensions::ManifestURL::GetDevToolsPage(extension->get()).is_empty())
      continue;
    DictionaryValue* extension_info = new DictionaryValue();
    extension_info->Set(
        "startPage",
        new StringValue(
            extensions::ManifestURL::GetDevToolsPage(extension->get()).spec()));
    extension_info->Set("name", new StringValue((*extension)->name()));
    extension_info->Set(
        "exposeExperimentalAPIs",
        new base::FundamentalValue((*extension)->HasAPIPermission(
            extensions::APIPermission::kExperimental)));
    results.Append(extension_info);
  }
  CallClientFunction("WebInspector.addExtensions", &results, NULL, NULL);
}
