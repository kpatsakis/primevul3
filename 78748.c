void DelegatedFrameHost::PrepareBitmapCopyOutputResult(
    const gfx::Size& dst_size_in_pixel,
    const SkColorType color_type,
    const base::Callback<void(bool, const SkBitmap&)>& callback,
    scoped_ptr<cc::CopyOutputResult> result) {
  if (color_type != kN32_SkColorType) {
    NOTIMPLEMENTED();
    callback.Run(false, SkBitmap());
    return;
  }
  DCHECK(result->HasBitmap());
  scoped_ptr<SkBitmap> source = result->TakeBitmap();
  DCHECK(source);
  SkBitmap bitmap = skia::ImageOperations::Resize(
      *source,
      skia::ImageOperations::RESIZE_BEST,
      dst_size_in_pixel.width(),
      dst_size_in_pixel.height());
  callback.Run(true, bitmap);
}
