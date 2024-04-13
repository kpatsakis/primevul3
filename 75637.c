Font* SubsetterImpl::FindFont(const char* font_name,
                              const FontArray& font_array) {
  if (font_array.empty() || font_array[0] == NULL) {
    return NULL;
  }

  if (font_name && strlen(font_name)) {
    for (FontArray::const_iterator b = font_array.begin(), e = font_array.end();
         b != e; ++b) {
      if (HasName(font_name, (*b).p_)) {
        return (*b).p_;
      }
    }
  }

  return font_array[0].p_;
}
