void WebContentsImpl::CreateViewAndSetSizeForRVH(RenderViewHost* rvh) {
  RenderWidgetHostView* rwh_view = view_->CreateViewForWidget(rvh);
  if (rwh_view)
    rwh_view->SetSize(GetView()->GetContainerSize());
}
