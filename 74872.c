void RenderView::UpdateTitle(WebFrame* frame, const string16& title) {
  if (!frame->parent()) {
    Send(new ViewHostMsg_UpdateTitle(
        routing_id_,
        page_id_,
        UTF16ToWideHack(title.length() > chrome::kMaxTitleChars ?
            title.substr(0, chrome::kMaxTitleChars) : title)));
  }
}
