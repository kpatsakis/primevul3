void RenderView::OnPepperPluginDestroy(
    WebPluginDelegatePepper* pepper_plugin) {
  std::set<WebPluginDelegatePepper*>::iterator found_pepper =
      current_oldstyle_pepper_plugins_.find(pepper_plugin);
  if (found_pepper == current_oldstyle_pepper_plugins_.end()) {
    NOTREACHED();
    return;
  }
  current_oldstyle_pepper_plugins_.erase(found_pepper);

  for (std::deque< linked_ptr<PendingFileChooser> >::iterator i =
           file_chooser_completions_.begin();
       i != file_chooser_completions_.end(); /* nothing */) {
    if ((*i)->completion == pepper_plugin) {
      if (i == file_chooser_completions_.begin())
        (*i)->completion = NULL;
      else
        i = file_chooser_completions_.erase(i);
    } else {
      ++i;
    }
  }
}
