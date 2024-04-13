void WebContentsImpl::GenerateMHTML(
    const base::FilePath& file,
    const base::Callback<void(int64)>& callback) {
  MHTMLGenerationManager::GetInstance()->SaveMHTML(this, file, callback);
}
