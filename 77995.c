void WebContentsImpl::CreateNewWidget(int route_id,
                                      WebKit::WebPopupType popup_type) {
  CreateNewWidget(route_id, false, popup_type);
}
