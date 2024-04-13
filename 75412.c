static void CopyPlane(size_t plane,
                      scoped_refptr<VideoFrame> video_frame,
                      const AVFrame* frame,
                      size_t source_height) {
  DCHECK_EQ(video_frame->width() % 2, 0u);
  const uint8* source = frame->data[plane];
  const size_t source_stride = frame->linesize[plane];
  uint8* dest = video_frame->data(plane);
  const size_t dest_stride = video_frame->stride(plane);

  size_t bytes_per_line = video_frame->width();
  size_t copy_lines = std::min(video_frame->height(), source_height);
  if (plane != VideoFrame::kYPlane) {
    bytes_per_line /= 2;
    if (video_frame->format() == VideoFrame::YV12) {
      copy_lines = (copy_lines + 1) / 2;
    }
  }
  bytes_per_line = std::min(bytes_per_line, source_stride);

  for (size_t i = 0; i < copy_lines; ++i) {
    memcpy(dest, source, bytes_per_line);
    source += source_stride;
    dest += dest_stride;
  }
}
