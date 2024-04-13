void RenderView::DidFlushPaint() {
  pepper_delegate_.ViewFlushedPaint();

  std::set<WebPluginDelegatePepper*> plugins = current_oldstyle_pepper_plugins_;
  for (std::set<WebPluginDelegatePepper*>::iterator i = plugins.begin();
       i != plugins.end(); ++i) {
    if (current_oldstyle_pepper_plugins_.find(*i) !=
        current_oldstyle_pepper_plugins_.end())
      (*i)->RenderViewFlushedPaint();
  }

  WebFrame* main_frame = webview()->mainFrame();

  if (!main_frame->provisionalDataSource()) {
    WebDataSource* ds = main_frame->dataSource();
    NavigationState* navigation_state = NavigationState::FromDataSource(ds);
    DCHECK(navigation_state);

    Time now = Time::Now();
    if (navigation_state->first_paint_time().is_null()) {
      navigation_state->set_first_paint_time(now);
    }
    if (navigation_state->first_paint_after_load_time().is_null() &&
        !navigation_state->finish_load_time().is_null()) {
      navigation_state->set_first_paint_after_load_time(now);
    }
  }
}
