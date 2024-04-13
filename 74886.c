WebMediaPlayer* RenderView::createMediaPlayer(
    WebFrame* frame, WebMediaPlayerClient* client) {
  scoped_refptr<media::FilterFactoryCollection> factory =
      new media::FilterFactoryCollection();
  const CommandLine* cmd_line = CommandLine::ForCurrentProcess();
  if (!cmd_line->HasSwitch(switches::kDisableAudio)) {
    factory->AddFactory(
        AudioRendererImpl::CreateFactory(audio_message_filter()));
  }

  if (cmd_line->HasSwitch(switches::kEnableAcceleratedDecoding) &&
      cmd_line->HasSwitch(switches::kEnableAcceleratedCompositing)) {
    factory->AddFactory(
        media::IpcVideoDecoder::CreateFactory(MessageLoop::current()));
  }

  WebApplicationCacheHostImpl* appcache_host =
      WebApplicationCacheHostImpl::FromFrame(frame);

  webkit_glue::MediaResourceLoaderBridgeFactory* bridge_factory =
      new webkit_glue::MediaResourceLoaderBridgeFactory(
          GURL(),  // referrer
          "null",  // frame origin
          "null",  // main_frame_origin
          base::GetCurrentProcId(),
          appcache_host ? appcache_host->host_id() : appcache::kNoHostId,
          routing_id());

  media::FilterFactory* simple_data_source_factory =
      webkit_glue::SimpleDataSource::CreateFactory(MessageLoop::current(),
                                                   bridge_factory);
  media::FilterFactory* buffered_data_source_factory =
      webkit_glue::BufferedDataSource::CreateFactory(MessageLoop::current(),
                                                     bridge_factory);
  if (cmd_line->HasSwitch(switches::kSimpleDataSource)) {
    factory->AddFactory(simple_data_source_factory);
    factory->AddFactory(buffered_data_source_factory);
  } else {
    factory->AddFactory(buffered_data_source_factory);
    factory->AddFactory(simple_data_source_factory);
  }

  webkit_glue::WebVideoRendererFactoryFactory* factory_factory = NULL;
  if (cmd_line->HasSwitch(switches::kEnableVideoLayering)) {
    factory_factory = new IPCVideoRenderer::FactoryFactory(routing_id_);
  } else {
    bool pts_logging = cmd_line->HasSwitch(switches::kEnableVideoLogging);
    factory_factory =
        new webkit_glue::VideoRendererImpl::FactoryFactory(pts_logging);
  }

  return new webkit_glue::WebMediaPlayerImpl(client, factory, factory_factory);
}
