void DelegatedFrameHost::DidReceiveFrameFromRenderer() {
  if (frame_subscriber() && CanCopyToVideoFrame()) {
    const base::TimeTicks present_time = base::TimeTicks::Now();
    scoped_refptr<media::VideoFrame> frame;
    RenderWidgetHostViewFrameSubscriber::DeliverFrameCallback callback;
    if (frame_subscriber()->ShouldCaptureFrame(present_time,
                                               &frame, &callback)) {
      CopyFromCompositingSurfaceToVideoFrame(
          gfx::Rect(current_frame_size_in_dip_),
          frame,
          base::Bind(callback, present_time));
    }
  }
}
