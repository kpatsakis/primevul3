void RenderFrameImpl::SetMediaStreamClientForTesting(
    MediaStreamClient* media_stream_client) {
  DCHECK(!media_stream_client_);
  DCHECK(!web_user_media_client_);
  media_stream_client_ = media_stream_client;
}
