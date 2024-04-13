void ChangeListLoader::UpdateAboutResourceAfterGetAbout(
    const google_apis::AboutResourceCallback& callback,
    google_apis::GDataErrorCode status,
    scoped_ptr<google_apis::AboutResource> about_resource) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  DCHECK(!callback.is_null());
  FileError error = GDataToFileError(status);

  if (error == FILE_ERROR_OK) {
    if (cached_about_resource_ &&
        cached_about_resource_->largest_change_id() >
        about_resource->largest_change_id()) {
      LOG(WARNING) << "Local cached about resource is fresher than server, "
                   << "local = " << cached_about_resource_->largest_change_id()
                   << ", server = " << about_resource->largest_change_id();
    }

    cached_about_resource_.reset(
        new google_apis::AboutResource(*about_resource));
  }

  callback.Run(status, about_resource.Pass());
}
