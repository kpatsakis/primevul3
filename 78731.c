void DelegatedFrameHost::CopyFromCompositingSurfaceHasResultForVideo(
    base::WeakPtr<DelegatedFrameHost> dfh,
    scoped_refptr<OwnedMailbox> subscriber_texture,
    scoped_refptr<media::VideoFrame> video_frame,
    const base::Callback<void(bool)>& callback,
    scoped_ptr<cc::CopyOutputResult> result) {
  base::ScopedClosureRunner scoped_callback_runner(base::Bind(callback, false));
  base::ScopedClosureRunner scoped_return_subscriber_texture(
      base::Bind(&ReturnSubscriberTexture, dfh, subscriber_texture, 0));

  if (!dfh)
    return;
  if (result->IsEmpty())
    return;
  if (result->size().IsEmpty())
    return;

  gfx::Rect region_in_frame =
      media::ComputeLetterboxRegion(gfx::Rect(video_frame->coded_size()),
                                    result->size());
  region_in_frame = gfx::Rect(region_in_frame.x() & ~1,
                              region_in_frame.y() & ~1,
                              region_in_frame.width() & ~1,
                              region_in_frame.height() & ~1);
  if (region_in_frame.IsEmpty())
    return;

  if (!result->HasTexture()) {
    DCHECK(result->HasBitmap());
    scoped_ptr<SkBitmap> bitmap = result->TakeBitmap();
    SkBitmap scaled_bitmap;
    if (result->size().width() != region_in_frame.width() ||
        result->size().height() != region_in_frame.height()) {
      skia::ImageOperations::ResizeMethod method =
          skia::ImageOperations::RESIZE_GOOD;
      scaled_bitmap = skia::ImageOperations::Resize(*bitmap.get(), method,
                                                    region_in_frame.width(),
                                                    region_in_frame.height());
    } else {
      scaled_bitmap = *bitmap.get();
    }

    {
      SkAutoLockPixels scaled_bitmap_locker(scaled_bitmap);

      media::CopyRGBToVideoFrame(
          reinterpret_cast<uint8*>(scaled_bitmap.getPixels()),
          scaled_bitmap.rowBytes(),
          region_in_frame,
          video_frame.get());
    }
    ignore_result(scoped_callback_runner.Release());
    callback.Run(true);
    return;
  }

  ImageTransportFactory* factory = ImageTransportFactory::GetInstance();
  GLHelper* gl_helper = factory->GetGLHelper();
  if (!gl_helper)
    return;
  if (subscriber_texture.get() && !subscriber_texture->texture_id())
    return;

  cc::TextureMailbox texture_mailbox;
  scoped_ptr<cc::SingleReleaseCallback> release_callback;
  result->TakeTexture(&texture_mailbox, &release_callback);
  DCHECK(texture_mailbox.IsTexture());

  gfx::Rect result_rect(result->size());

  content::ReadbackYUVInterface* yuv_readback_pipeline =
      dfh->yuv_readback_pipeline_.get();
  if (yuv_readback_pipeline == NULL ||
      yuv_readback_pipeline->scaler()->SrcSize() != result_rect.size() ||
      yuv_readback_pipeline->scaler()->SrcSubrect() != result_rect ||
      yuv_readback_pipeline->scaler()->DstSize() != region_in_frame.size()) {
    GLHelper::ScalerQuality quality = GLHelper::SCALER_QUALITY_FAST;
    std::string quality_switch = switches::kTabCaptureDownscaleQuality;
    if (result_rect.size().width() < region_in_frame.size().width() &&
        result_rect.size().height() < region_in_frame.size().height())
      quality_switch = switches::kTabCaptureUpscaleQuality;

    std::string switch_value =
        CommandLine::ForCurrentProcess()->GetSwitchValueASCII(quality_switch);
    if (switch_value == "fast")
      quality = GLHelper::SCALER_QUALITY_FAST;
    else if (switch_value == "good")
      quality = GLHelper::SCALER_QUALITY_GOOD;
    else if (switch_value == "best")
      quality = GLHelper::SCALER_QUALITY_BEST;

    dfh->yuv_readback_pipeline_.reset(
        gl_helper->CreateReadbackPipelineYUV(quality,
                                             result_rect.size(),
                                             result_rect,
                                             video_frame->coded_size(),
                                             region_in_frame,
                                             true,
                                             true));
    yuv_readback_pipeline = dfh->yuv_readback_pipeline_.get();
  }

  ignore_result(scoped_callback_runner.Release());
  ignore_result(scoped_return_subscriber_texture.Release());
  base::Callback<void(bool result)> finished_callback = base::Bind(
      &DelegatedFrameHost::CopyFromCompositingSurfaceFinishedForVideo,
      dfh->AsWeakPtr(),
      callback,
      subscriber_texture,
      base::Passed(&release_callback));
  yuv_readback_pipeline->ReadbackYUV(texture_mailbox.mailbox(),
                                     texture_mailbox.sync_point(),
                                     video_frame.get(),
                                     finished_callback);
}
