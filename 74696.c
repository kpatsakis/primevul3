void AutoFillManager::OnInfoBarClosed(bool should_save) {
  if (should_save)
    personal_data_->SaveImportedCreditCard();
  UploadFormData();
}
