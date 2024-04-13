LRESULT OmniboxViewWin::OnGetObject(UINT message,
                                    WPARAM wparam,
                                    LPARAM lparam) {
  if (lparam == OBJID_CLIENT) {
    return LresultFromObject(IID_IAccessible, wparam,
                             native_view_host_->GetNativeViewAccessible());
  }
  return 0;
}
