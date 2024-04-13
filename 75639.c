bool SubsetterImpl::LoadFont(const char* font_name,
                             const unsigned char* original_font,
                             size_t font_size) {
  ByteArrayPtr raw_font =
      new MemoryByteArray((byte_t*)original_font, font_size);
  if (factory_ == NULL) {
    factory_.Attach(FontFactory::GetInstance());
  }

  FontArray font_array;
  factory_->LoadFonts(raw_font, &font_array);
  font_ = FindFont(font_name, font_array);
  if (font_ == NULL) {
    return false;
  }

  return true;
}
