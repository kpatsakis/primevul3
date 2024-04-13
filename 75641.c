int SubsetterImpl::SubsetFont(const unsigned int* glyph_ids,
                              size_t glyph_count,
                              unsigned char** output_buffer) {
  if (factory_ == NULL || font_ == NULL) {
    return -1;
  }

  IntegerSet glyph_id_processed;
  if (!ResolveCompositeGlyphs(glyph_ids, glyph_count, &glyph_id_processed) ||
      glyph_id_processed.empty()) {
    return 0;
  }

  FontPtr new_font;
  new_font.Attach(Subset(glyph_id_processed));
  if (new_font == NULL) {
    return 0;
  }

  MemoryOutputStream output_stream;
  factory_->SerializeFont(new_font, &output_stream);
  int length = static_cast<int>(output_stream.Size());
  if (length > 0) {
    *output_buffer = new unsigned char[length];
    memcpy(*output_buffer, output_stream.Get(), length);
  }

  return length;
}
