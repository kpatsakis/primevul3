WebWidget* RenderView::createPopupMenu(WebKit::WebPopupType popup_type) {
  RenderWidget* widget = RenderWidget::Create(routing_id_,
                                              render_thread_,
                                              popup_type);
  return widget->webwidget();
}
