void RenderView::setKeyboardFocusURL(const WebURL& url) {
  focus_url_ = GURL(url);
  UpdateTargetURL(focus_url_, mouse_over_url_);
}
