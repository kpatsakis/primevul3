void RenderView::DidMovePlugin(const webkit_glue::WebPluginGeometry& move) {
  SchedulePluginMove(move);
}
