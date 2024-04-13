blink::WebMediaPlayer* RenderFrameImpl::createMediaPlayer(
    blink::WebLocalFrame* frame,
    const blink::WebURL& url,
    blink::WebMediaPlayerClient* client) {
  WebMediaPlayer* player = CreateWebMediaPlayerForMediaStream(url, client);
  if (player)
    return player;

#if defined(OS_ANDROID)
  return CreateAndroidWebMediaPlayer(url, client);
#else
  WebMediaPlayerParams params(
      base::Bind(&ContentRendererClient::DeferMediaLoad,
                 base::Unretained(GetContentClient()->renderer()),
                 static_cast<RenderFrame*>(this)),
      RenderThreadImpl::current()->GetAudioRendererMixerManager()->CreateInput(
          render_view_->routing_id_, routing_id_));
  return new WebMediaPlayerImpl(frame, client, weak_factory_.GetWeakPtr(),
                                params);
#endif  // defined(OS_ANDROID)
}
