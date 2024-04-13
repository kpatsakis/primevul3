void RenderView::OnSetAccessibilityFocus(int acc_obj_id) {
  if (!accessibility_.get())
    return;
  if (accessibility_->isValidId(acc_obj_id)) {
    if (acc_obj_id == 1000)
      webview()->clearFocusedNode();
    else
      accessibility_->getObjectById(acc_obj_id).setFocused(true);
  }
}
