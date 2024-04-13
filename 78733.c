DelegatedFrameHost::DelegatedFrameHost(DelegatedFrameHostClient* client)
    : client_(client),
      use_surfaces_(base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kUseSurfaces)),
      last_output_surface_id_(0),
      pending_delegated_ack_count_(0),
      skipped_frames_(false),
      can_lock_compositor_(YES_CAN_LOCK),
      delegated_frame_evictor_(new DelegatedFrameEvictor(this)) {
  ImageTransportFactory::GetInstance()->AddObserver(this);
}
