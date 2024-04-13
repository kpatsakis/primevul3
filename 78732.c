void DelegatedFrameHost::CopyFromCompositingSurfaceToVideoFrame(
      const gfx::Rect& src_subrect,
      const scoped_refptr<media::VideoFrame>& target,
      const base::Callback<void(bool)>& callback) {
  if (!CanCopyToVideoFrame()) {
    callback.Run(false);
    return;
  }

  scoped_refptr<OwnedMailbox> subscriber_texture;
  if (frame_subscriber_) {
    if (!idle_frame_subscriber_textures_.empty()) {
      subscriber_texture = idle_frame_subscriber_textures_.back();
      idle_frame_subscriber_textures_.pop_back();
    } else if (GLHelper* helper =
                   ImageTransportFactory::GetInstance()->GetGLHelper()) {
      subscriber_texture = new OwnedMailbox(helper);
    }
  }

  scoped_ptr<cc::CopyOutputRequest> request =
      cc::CopyOutputRequest::CreateRequest(base::Bind(
          &DelegatedFrameHost::
               CopyFromCompositingSurfaceHasResultForVideo,
          AsWeakPtr(),  // For caching the ReadbackYUVInterface on this class.
          subscriber_texture,
          target,
          callback));
  gfx::Rect src_subrect_in_pixel =
      ConvertRectToPixel(client_->CurrentDeviceScaleFactor(), src_subrect);
  request->set_area(src_subrect_in_pixel);
  if (subscriber_texture.get()) {
    request->SetTextureMailbox(
        cc::TextureMailbox(subscriber_texture->mailbox(),
                           subscriber_texture->target(),
                           subscriber_texture->sync_point()));
  }
  client_->RequestCopyOfOutput(request.Pass());
}
