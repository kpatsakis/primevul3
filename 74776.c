void RenderView::OnCSSInsertRequest(const std::wstring& frame_xpath,
                                    const std::string& css,
                                    const std::string& id) {
  InsertCSS(frame_xpath, css, id);

  Send(new ViewHostMsg_OnCSSInserted(routing_id_));
}
