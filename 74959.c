void RenderView::show(WebNavigationPolicy policy) {
  DCHECK(!did_show_) << "received extraneous Show call";
  DCHECK(opener_id_ != MSG_ROUTING_NONE);

  if (did_show_)
    return;
  did_show_ = true;

  if (!opened_by_user_gesture_) {
    if (policy != WebKit::WebNavigationPolicyNewBackgroundTab)
      policy = WebKit::WebNavigationPolicyNewPopup;
  }

  Send(new ViewHostMsg_ShowView(opener_id_, routing_id_,
      NavigationPolicyToDisposition(policy), initial_pos_,
      opened_by_user_gesture_));
  SetPendingWindowRect(initial_pos_);
}
