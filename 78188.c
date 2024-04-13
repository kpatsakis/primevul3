void WebContentsImpl::ShowCreatedWindow(int route_id,
                                        WindowOpenDisposition disposition,
                                        const gfx::Rect& initial_pos,
                                        bool user_gesture) {
  WebContentsImpl* contents = GetCreatedWindow(route_id);
  if (contents) {
    WebContentsDelegate* delegate = GetDelegate();
    if (delegate) {
      delegate->AddNewContents(
          this, contents, disposition, initial_pos, user_gesture, NULL);
    }
  }
}
