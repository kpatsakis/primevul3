url_parse::Component UTF8ComponentToUTF16Component(
    const std::string& text_utf8,
    const url_parse::Component& component_utf8) {
  if (component_utf8.len == -1)
    return url_parse::Component();

  std::string before_component_string =
      text_utf8.substr(0, component_utf8.begin);
  std::string component_string = text_utf8.substr(component_utf8.begin,
                                                  component_utf8.len);
  string16 before_component_string_16 = UTF8ToUTF16(before_component_string);
  string16 component_string_16 = UTF8ToUTF16(component_string);
  url_parse::Component component_16(before_component_string_16.length(),
                                    component_string_16.length());
  return component_16;
}
