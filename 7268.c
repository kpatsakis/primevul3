static void ConvertHeader(EXRHeader *exr_header, const HeaderInfo &info) {
  exr_header->pixel_aspect_ratio = info.pixel_aspect_ratio;
  exr_header->screen_window_center[0] = info.screen_window_center[0];
  exr_header->screen_window_center[1] = info.screen_window_center[1];
  exr_header->screen_window_width = info.screen_window_width;
  exr_header->chunk_count = info.chunk_count;
  exr_header->display_window[0] = info.display_window[0];
  exr_header->display_window[1] = info.display_window[1];
  exr_header->display_window[2] = info.display_window[2];
  exr_header->display_window[3] = info.display_window[3];
  exr_header->data_window[0] = info.data_window[0];
  exr_header->data_window[1] = info.data_window[1];
  exr_header->data_window[2] = info.data_window[2];
  exr_header->data_window[3] = info.data_window[3];
  exr_header->line_order = info.line_order;
  exr_header->compression_type = info.compression_type;

  exr_header->tile_size_x = info.tile_size_x;
  exr_header->tile_size_y = info.tile_size_y;
  exr_header->tile_level_mode = info.tile_level_mode;
  exr_header->tile_rounding_mode = info.tile_rounding_mode;

  exr_header->num_channels = static_cast<int>(info.channels.size());

  exr_header->channels = static_cast<EXRChannelInfo *>(malloc(
      sizeof(EXRChannelInfo) * static_cast<size_t>(exr_header->num_channels)));
  for (size_t c = 0; c < static_cast<size_t>(exr_header->num_channels); c++) {
#ifdef _MSC_VER
    strncpy_s(exr_header->channels[c].name, info.channels[c].name.c_str(), 255);
#else
    strncpy(exr_header->channels[c].name, info.channels[c].name.c_str(), 255);
#endif
    // manually add '\0' for safety.
    exr_header->channels[c].name[255] = '\0';

    exr_header->channels[c].pixel_type = info.channels[c].pixel_type;
    exr_header->channels[c].p_linear = info.channels[c].p_linear;
    exr_header->channels[c].x_sampling = info.channels[c].x_sampling;
    exr_header->channels[c].y_sampling = info.channels[c].y_sampling;
  }

  exr_header->pixel_types = static_cast<int *>(
      malloc(sizeof(int) * static_cast<size_t>(exr_header->num_channels)));
  for (size_t c = 0; c < static_cast<size_t>(exr_header->num_channels); c++) {
    exr_header->pixel_types[c] = info.channels[c].pixel_type;
  }

  // Initially fill with values of `pixel_types`
  exr_header->requested_pixel_types = static_cast<int *>(
      malloc(sizeof(int) * static_cast<size_t>(exr_header->num_channels)));
  for (size_t c = 0; c < static_cast<size_t>(exr_header->num_channels); c++) {
    exr_header->requested_pixel_types[c] = info.channels[c].pixel_type;
  }

  exr_header->num_custom_attributes = static_cast<int>(info.attributes.size());

  if (exr_header->num_custom_attributes > 0) {
    // TODO(syoyo): Report warning when # of attributes exceeds
    // `TINYEXR_MAX_CUSTOM_ATTRIBUTES`
    if (exr_header->num_custom_attributes > TINYEXR_MAX_CUSTOM_ATTRIBUTES) {
      exr_header->num_custom_attributes = TINYEXR_MAX_CUSTOM_ATTRIBUTES;
    }

    exr_header->custom_attributes = static_cast<EXRAttribute *>(malloc(
        sizeof(EXRAttribute) * size_t(exr_header->num_custom_attributes)));

    for (size_t i = 0; i < info.attributes.size(); i++) {
      memcpy(exr_header->custom_attributes[i].name, info.attributes[i].name,
             256);
      memcpy(exr_header->custom_attributes[i].type, info.attributes[i].type,
             256);
      exr_header->custom_attributes[i].size = info.attributes[i].size;
      // Just copy poiner
      exr_header->custom_attributes[i].value = info.attributes[i].value;
    }

  } else {
    exr_header->custom_attributes = NULL;
  }

  exr_header->header_len = info.header_len;
}