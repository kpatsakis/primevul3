void DelegatedFrameHost::SwapDelegatedFrame(
    uint32 output_surface_id,
    scoped_ptr<cc::DelegatedFrameData> frame_data,
    float frame_device_scale_factor,
    const std::vector<ui::LatencyInfo>& latency_info) {
  RenderWidgetHostImpl* host = client_->GetHost();
  DCHECK(!frame_data->render_pass_list.empty());

  cc::RenderPass* root_pass = frame_data->render_pass_list.back();

  gfx::Size frame_size = root_pass->output_rect.size();
  gfx::Size frame_size_in_dip =
      ConvertSizeToDIP(frame_device_scale_factor, frame_size);

  gfx::Rect damage_rect = gfx::ToEnclosingRect(root_pass->damage_rect);
  damage_rect.Intersect(gfx::Rect(frame_size));
  gfx::Rect damage_rect_in_dip =
      ConvertRectToDIP(frame_device_scale_factor, damage_rect);

  if (ShouldSkipFrame(frame_size_in_dip)) {
    cc::CompositorFrameAck ack;
    cc::TransferableResource::ReturnResources(frame_data->resource_list,
                                              &ack.resources);

    skipped_latency_info_list_.insert(skipped_latency_info_list_.end(),
        latency_info.begin(), latency_info.end());

    RenderWidgetHostImpl::SendSwapCompositorFrameAck(
        host->GetRoutingID(), output_surface_id,
        host->GetProcess()->GetID(), ack);
    skipped_frames_ = true;
    return;
  }

  if (skipped_frames_) {
    skipped_frames_ = false;
    damage_rect = gfx::Rect(frame_size);
    damage_rect_in_dip = gfx::Rect(frame_size_in_dip);

    cc::RenderPass* root_pass = frame_data->render_pass_list.back();
    root_pass->damage_rect = damage_rect;
  }

  if (output_surface_id != last_output_surface_id_) {
    EvictDelegatedFrame();

    if (resource_collection_.get()) {
      resource_collection_->SetClient(NULL);

      if (resource_collection_->LoseAllResources())
        SendReturnedDelegatedResources(last_output_surface_id_);

      resource_collection_ = NULL;
    }
    last_output_surface_id_ = output_surface_id;
  }
  if (frame_size.IsEmpty()) {
    DCHECK(frame_data->resource_list.empty());
    EvictDelegatedFrame();
  } else {
    if (use_surfaces_) {
      if (!surface_factory_) {
        ImageTransportFactory* factory = ImageTransportFactory::GetInstance();
        cc::SurfaceManager* manager = factory->GetSurfaceManager();
        id_allocator_ = factory->CreateSurfaceIdAllocator();
        surface_factory_ =
            make_scoped_ptr(new cc::SurfaceFactory(manager, this));
      }
      if (surface_id_.is_null() || frame_size != current_surface_size_ ||
          frame_size_in_dip != current_frame_size_in_dip_) {
        if (!surface_id_.is_null())
          surface_factory_->Destroy(surface_id_);
        surface_id_ = id_allocator_->GenerateId();
        surface_factory_->Create(surface_id_, frame_size);
        client_->GetLayer()->SetShowSurface(surface_id_, frame_size_in_dip);
        current_surface_size_ = frame_size;
      }
      scoped_ptr<cc::CompositorFrame> compositor_frame =
          make_scoped_ptr(new cc::CompositorFrame());
      compositor_frame->delegated_frame_data = frame_data.Pass();
      surface_factory_->SubmitFrame(surface_id_, compositor_frame.Pass());
    } else {
      if (!resource_collection_) {
        resource_collection_ = new cc::DelegatedFrameResourceCollection;
        resource_collection_->SetClient(this);
      }
      if (!frame_provider_.get() ||
          frame_size != frame_provider_->frame_size() ||
          frame_size_in_dip != current_frame_size_in_dip_) {
        frame_provider_ = new cc::DelegatedFrameProvider(
            resource_collection_.get(), frame_data.Pass());
        client_->GetLayer()->SetShowDelegatedContent(frame_provider_.get(),
                                                     frame_size_in_dip);
      } else {
        frame_provider_->SetFrameData(frame_data.Pass());
      }
    }
  }
  released_front_lock_ = NULL;
  current_frame_size_in_dip_ = frame_size_in_dip;
  CheckResizeLock();

  client_->SchedulePaintInRect(damage_rect_in_dip);

  pending_delegated_ack_count_++;

  ui::Compositor* compositor = client_->GetCompositor();
  if (!compositor) {
    SendDelegatedFrameAck(output_surface_id);
  } else {
    std::vector<ui::LatencyInfo>::const_iterator it;
    for (it = latency_info.begin(); it != latency_info.end(); ++it)
      compositor->SetLatencyInfo(*it);
    for (it = skipped_latency_info_list_.begin();
        it != skipped_latency_info_list_.end();
        ++it)
      compositor->SetLatencyInfo(*it);
    skipped_latency_info_list_.clear();
    AddOnCommitCallbackAndDisableLocks(
        base::Bind(&DelegatedFrameHost::SendDelegatedFrameAck,
                   AsWeakPtr(),
                   output_surface_id));
  }
  DidReceiveFrameFromRenderer();
  if (frame_provider_.get() || !surface_id_.is_null())
    delegated_frame_evictor_->SwappedFrame(!host->is_hidden());
}
