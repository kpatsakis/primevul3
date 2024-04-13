void RenderView::OnSetDOMUIProperty(const std::string& name,
                                    const std::string& value) {
  DCHECK(BindingsPolicy::is_dom_ui_enabled(enabled_bindings_));
  dom_ui_bindings_.SetProperty(name, value);
}
