void RenderView::closeWidgetSoon() {
  translate_helper_.CancelPendingTranslation();

  if (script_can_close_)
    RenderWidget::closeWidgetSoon();
}
