  void CancelModalDialog() {
    NativeAppModalDialog* native_dialog = GetDialog();
    native_dialog->CancelAppModalDialog();
  }
