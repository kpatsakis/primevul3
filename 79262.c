bool ExecuteCodeInTabFunction::RunImpl() {
  EXTENSION_FUNCTION_VALIDATE(Init());

  if (!details_->code.get() && !details_->file.get()) {
    error_ = keys::kNoCodeOrFileToExecuteError;
    return false;
  }
  if (details_->code.get() && details_->file.get()) {
    error_ = keys::kMoreThanOneValuesError;
    return false;
  }

  content::WebContents* contents = NULL;

  CHECK_GE(execute_tab_id_, 0);
  if (!GetTabById(execute_tab_id_, profile(),
                  include_incognito(),
                  NULL, NULL, &contents, NULL, &error_)) {
    return false;
  }

  CHECK(contents);
  if (!GetExtension()->CanExecuteScriptOnPage(contents->GetURL(),
                                              contents->GetURL(),
                                              execute_tab_id_,
                                              NULL,
                                              &error_)) {
    return false;
  }

  if (details_->code.get())
    return Execute(*details_->code);

  CHECK(details_->file.get());
  resource_ = GetExtension()->GetResource(*details_->file);

  if (resource_.extension_root().empty() || resource_.relative_path().empty()) {
    error_ = keys::kNoCodeOrFileToExecuteError;
    return false;
  }

  scoped_refptr<FileReader> file_reader(new FileReader(
      resource_, base::Bind(&ExecuteCodeInTabFunction::DidLoadFile, this)));
  file_reader->Start();

  return true;
}
