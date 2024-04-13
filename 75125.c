std::wstring IDNToUnicode(const char* host,
                          size_t host_len,
                          const std::wstring& languages,
                          size_t* offset_for_adjustment) {
  string16 input16;
  input16.reserve(host_len);
  std::copy(host, host + host_len, std::back_inserter(input16));

  string16 out16;
  size_t output_offset = offset_for_adjustment ?
      *offset_for_adjustment : std::wstring::npos;

  for (size_t component_start = 0, component_end;
       component_start < input16.length();
       component_start = component_end + 1) {
    component_end = input16.find('.', component_start);
    if (component_end == string16::npos)
      component_end = input16.length();  // For getting the last component.
    size_t component_length = component_end - component_start;

    size_t output_component_start = out16.length();
    bool converted_idn = false;
    if (component_end > component_start) {
      converted_idn = IDNToUnicodeOneComponent(input16.data() + component_start,
          component_length, languages, &out16);
    }
    size_t output_component_length = out16.length() - output_component_start;

    if ((output_offset != std::wstring::npos) &&
        (*offset_for_adjustment > component_start)) {
      if ((*offset_for_adjustment < component_end) && converted_idn)
        output_offset = std::wstring::npos;
      else
        output_offset += output_component_length - component_length;
    }

    if (component_end < input16.length())
      out16.push_back('.');
  }

  if (offset_for_adjustment)
    *offset_for_adjustment = output_offset;

  return UTF16ToWideAndAdjustOffset(out16, offset_for_adjustment);
}
