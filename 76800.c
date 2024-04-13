  void LoadImageCursor(int id, int resource_id) {
    const SkBitmap* bitmap =
        ui::ResourceBundle::GetSharedInstance().GetImageNamed(
            resource_id).ToSkBitmap();

    XcursorImage* image = ui::SkBitmapToXcursorImage(bitmap, gfx::Point(0, 0));
    cursors_[id] = ui::CreateReffedCustomXCursor(image);
  }
