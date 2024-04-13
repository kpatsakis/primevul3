  void clear() {
    channels.clear();
    attributes.clear();

    data_window[0] = 0;
    data_window[1] = 0;
    data_window[2] = 0;
    data_window[3] = 0;
    line_order = 0;
    display_window[0] = 0;
    display_window[1] = 0;
    display_window[2] = 0;
    display_window[3] = 0;
    screen_window_center[0] = 0.0f;
    screen_window_center[1] = 0.0f;
    screen_window_width = 0.0f;
    pixel_aspect_ratio = 0.0f;

    chunk_count = 0;

    // Tiled format
    tile_size_x = 0;
    tile_size_y = 0;
    tile_level_mode = 0;
    tile_rounding_mode = 0;

    header_len = 0;
    compression_type = 0;
  }