void RenderView::ShowModalHTMLDialogForPlugin(
    const GURL& url,
    const gfx::Size& size,
    const std::string& json_arguments,
    std::string* json_retval) {
  SendAndRunNestedMessageLoop(new ViewHostMsg_ShowModalHTMLDialog(
      routing_id_, url, size.width(), size.height(), json_arguments,
      json_retval));
}
