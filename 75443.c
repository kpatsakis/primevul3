void ExtensionPrefs::SetBrowserActionVisibility(const Extension* extension,
                                                bool visible) {
  if (GetBrowserActionVisibility(extension) == visible)
    return;

  UpdateExtensionPref(extension->id(), kBrowserActionVisible,
                      Value::CreateBooleanValue(visible));
  NotificationService::current()->Notify(
      NotificationType::EXTENSION_BROWSER_ACTION_VISIBILITY_CHANGED,
      Source<ExtensionPrefs>(this),
      Details<const Extension>(extension));
}
