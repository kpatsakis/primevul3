static void MakeThrobberFrames(int resource_id,
                               std::vector<GdkPixbuf*>* frames) {
  ResourceBundle &rb = ResourceBundle::GetSharedInstance();
  SkBitmap* frame_strip = rb.GetBitmapNamed(resource_id);

  int frame_size = frame_strip->height();
  size_t num_frames = frame_strip->width() / frame_size;

  for (size_t i = 0; i < num_frames; ++i) {
    SkBitmap frame = SkBitmapOperations::CreateTiledBitmap(*frame_strip,
        i * frame_size, 0, frame_size, frame_size);
    frames->push_back(gfx::GdkPixbufFromSkBitmap(&frame));
  }
}
