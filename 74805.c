void RenderView::OnGetApplicationInfo(int page_id) {
  webkit_glue::WebApplicationInfo app_info;
  if (page_id == page_id_)
    webkit_glue::GetApplicationInfo(webview(), &app_info);

  for (size_t i = 0; i < app_info.icons.size(); ++i) {
    if (app_info.icons[i].url.SchemeIs(chrome::kDataScheme)) {
      app_info.icons.erase(app_info.icons.begin() + i);
      --i;
    }
  }

  Send(new ViewHostMsg_DidGetApplicationInfo(routing_id_, page_id, app_info));
}
