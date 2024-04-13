RenderView::GetSearchProviderInstallState(const std::string& url) {
  GURL inquiry_url = GURL(url);
  if (inquiry_url.is_empty())
    return ViewHostMsg_GetSearchProviderInstallState_Params::Denied();
  ViewHostMsg_GetSearchProviderInstallState_Params install;
  Send(new ViewHostMsg_GetSearchProviderInstallState(routing_id_,
                                                     inquiry_url,
                                                     &install));
  return install;
}
