static void DecodeTiledPixelData(
    unsigned char **out_images, int *width, int *height,
    const int *requested_pixel_types, const unsigned char *data_ptr,
    size_t data_len, int compression_type, int line_order, int data_width,
    int data_height, int tile_offset_x, int tile_offset_y, int tile_size_x,
    int tile_size_y, size_t pixel_data_size, size_t num_attributes,
    const EXRAttribute *attributes, size_t num_channels,
    const EXRChannelInfo *channels,
    const std::vector<size_t> &channel_offset_list) {
  assert(tile_offset_x * tile_size_x < data_width);
  assert(tile_offset_y * tile_size_y < data_height);

  // Compute actual image size in a tile.
  if ((tile_offset_x + 1) * tile_size_x >= data_width) {
    (*width) = data_width - (tile_offset_x * tile_size_x);
  } else {
    (*width) = tile_size_x;
  }

  if ((tile_offset_y + 1) * tile_size_y >= data_height) {
    (*height) = data_height - (tile_offset_y * tile_size_y);
  } else {
    (*height) = tile_size_y;
  }

  // Image size = tile size.
  DecodePixelData(out_images, requested_pixel_types, data_ptr, data_len,
                  compression_type, line_order, (*width), tile_size_y,
                  /* stride */ tile_size_x, /* y */ 0, /* line_no */ 0,
                  (*height), pixel_data_size, num_attributes, attributes,
                  num_channels, channels, channel_offset_list);
}