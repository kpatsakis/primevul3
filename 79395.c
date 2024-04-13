void ChangeListLoader::UpdateAboutResource(
    const google_apis::AboutResourceCallback& callback) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  DCHECK(!callback.is_null());

  scheduler_->GetAboutResource(
      base::Bind(&ChangeListLoader::UpdateAboutResourceAfterGetAbout,
                 weak_ptr_factory_.GetWeakPtr(),
                 callback));
}
