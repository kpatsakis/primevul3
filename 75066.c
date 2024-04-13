TabContentsView* InterstitialPage::CreateTabContentsView() {
  TabContentsView* tab_contents_view = tab()->view();
  RenderWidgetHostView* view =
      tab_contents_view->CreateViewForWidget(render_view_host_);
  render_view_host_->set_view(view);
  render_view_host_->AllowBindings(BindingsPolicy::DOM_AUTOMATION);

  scoped_refptr<URLRequestContextGetter> request_context =
      tab()->request_context();
  if (!request_context.get())
    request_context = tab()->profile()->GetRequestContext();

  render_view_host_->CreateRenderView(request_context.get());
  view->SetSize(tab_contents_view->GetContainerSize());
  view->Hide();
  return tab_contents_view;
}
