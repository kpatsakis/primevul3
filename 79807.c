bool ChromeDownloadManagerDelegate::ShouldOpenFileBasedOnExtension(
    const FilePath& path) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  FilePath::StringType extension = path.Extension();
  if (extension.empty())
    return false;
  if (extensions::Extension::IsExtension(path))
    return false;
  DCHECK(extension[0] == FilePath::kExtensionSeparator);
  extension.erase(0, 1);
  return download_prefs_->IsAutoOpenEnabledForExtension(extension);
}
