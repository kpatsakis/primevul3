void AutoFillManager::ShowAutoFillDialog() {
  ::ShowAutoFillDialog(tab_contents_->GetContentNativeView(),
                       personal_data_,
                       tab_contents_->profile()->GetOriginalProfile());
}
