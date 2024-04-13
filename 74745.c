void RenderView::DidInitiatePaint() {
  pepper_delegate_.ViewInitiatedPaint();

  for (std::set<WebPluginDelegatePepper*>::iterator i =
           current_oldstyle_pepper_plugins_.begin();
       i != current_oldstyle_pepper_plugins_.end(); ++i)
    (*i)->RenderViewInitiatedPaint();
}
