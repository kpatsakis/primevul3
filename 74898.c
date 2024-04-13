void RenderView::didChangeAccessibilityObjectState(
    const WebKit::WebAccessibilityObject& acc_obj) {
#if defined(OS_WIN)
  if (!accessibility_.get())
    return;

  int acc_obj_id = accessibility_->addOrGetId(acc_obj);

  if (acc_obj_id >= 0)
    Send(new ViewHostMsg_AccessibilityObjectStateChange(routing_id_,
                                                        acc_obj_id));

#else  // defined(OS_WIN)
  NOTIMPLEMENTED();
#endif
}
