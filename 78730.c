void DelegatedFrameHost::CopyFromCompositingSurfaceFinishedForVideo(
    base::WeakPtr<DelegatedFrameHost> dfh,
    const base::Callback<void(bool)>& callback,
    scoped_refptr<OwnedMailbox> subscriber_texture,
    scoped_ptr<cc::SingleReleaseCallback> release_callback,
    bool result) {
  callback.Run(result);

  uint32 sync_point = 0;
  if (result) {
    GLHelper* gl_helper = ImageTransportFactory::GetInstance()->GetGLHelper();
    sync_point = gl_helper->InsertSyncPoint();
  }
  if (release_callback) {
    DCHECK(!subscriber_texture);
    bool lost_resource = sync_point == 0;
    release_callback->Run(sync_point, lost_resource);
  }
  ReturnSubscriberTexture(dfh, subscriber_texture, sync_point);
}
