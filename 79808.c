bool ChromeDownloadManagerDelegate::ShouldOpenWithWebIntents(
    const DownloadItem* item) {
  if (!web_intents::IsWebIntentsEnabledForProfile(profile_))
    return false;

  if ((item->GetWebContents() && !item->GetWebContents()->GetDelegate()) &&
      !web_intents::GetBrowserForBackgroundWebIntentDelivery(profile_)) {
    return false;
  }
  if (!item->GetForcedFilePath().empty())
    return false;
  if (item->GetTargetDisposition() == DownloadItem::TARGET_DISPOSITION_PROMPT)
    return false;

#if !defined(OS_CHROMEOS)
  std::string mime_type = item->GetMimeType();

  const char kQuickOfficeExtensionId[] = "gbkeegbaiigmenfmjfclcdgdpimamgkj";
  const char kQuickOfficeDevExtensionId[] = "ionpfmkccalenbmnddpbmocokhaknphg";
  ExtensionServiceInterface* extension_service =
      extensions::ExtensionSystem::Get(profile_)->extension_service();

  bool use_quickoffice = false;
  if (extension_service &&
      extension_service->GetInstalledExtension(kQuickOfficeExtensionId) &&
      extension_service->IsExtensionEnabled(kQuickOfficeExtensionId))
    use_quickoffice = true;

  if (extension_service &&
      extension_service->GetInstalledExtension(kQuickOfficeDevExtensionId) &&
      extension_service->IsExtensionEnabled(kQuickOfficeDevExtensionId))
    use_quickoffice = true;

  if (use_quickoffice) {
    if (mime_type == "application/msword" ||
        mime_type == "application/vnd.ms-powerpoint" ||
        mime_type == "application/vnd.ms-excel" ||
        mime_type == "application/vnd.openxmlformats-officedocument."
                     "wordprocessingml.document" ||
        mime_type == "application/vnd.openxmlformats-officedocument."
                     "presentationml.presentation" ||
        mime_type == "application/vnd.openxmlformats-officedocument."
                     "spreadsheetml.sheet") {
      return true;
    }
  }
#endif  // !defined(OS_CHROMEOS)

  return false;
}
