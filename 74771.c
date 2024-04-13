void RenderView::OnAccessibilityDoDefaultAction(int acc_obj_id) {
  if (!accessibility_.get())
    return;
  if (accessibility_->isValidId(acc_obj_id)) {
    accessibility_->getObjectById(acc_obj_id).performDefaultAction();
  }
}
