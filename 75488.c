void ExtensionService::CheckForExternalUpdates() {
  CHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));


  external_extension_url_added_ = false;

  ProviderCollection::const_iterator i;
  for (i = external_extension_providers_.begin();
       i != external_extension_providers_.end(); ++i) {
    ExternalExtensionProviderInterface* provider = i->get();
    provider->VisitRegisteredExtension();
  }

  if (external_extension_providers_.empty())
    OnExternalProviderReady();
}
