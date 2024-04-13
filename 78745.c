void DelegatedFrameHost::OnLayerRecreated(ui::Layer* old_layer,
                                                ui::Layer* new_layer) {
  if (frame_provider_.get()) {
    new_layer->SetShowDelegatedContent(frame_provider_.get(),
                                       current_frame_size_in_dip_);
  }
  if (!surface_id_.is_null()) {
    new_layer->SetShowSurface(surface_id_, current_frame_size_in_dip_);
  }
}
