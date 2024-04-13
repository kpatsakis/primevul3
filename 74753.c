void RenderView::ForEach(RenderViewVisitor* visitor) {
  ViewMap* views = Singleton<ViewMap>::get();
  for (ViewMap::iterator it = views->begin(); it != views->end(); ++it) {
    if (!visitor->Visit(it->second))
      return;
  }
}
