void DelegatedFrameHost::OnUpdateVSyncParameters(
    base::TimeTicks timebase,
    base::TimeDelta interval) {
  RenderWidgetHostImpl* host = client_->GetHost();
  if (client_->IsVisible())
    host->UpdateVSyncParameters(timebase, interval);
}
