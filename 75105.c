void AppendFormattedComponent(const std::string& spec,
                              const url_parse::Component& in_component,
                              UnescapeRule::Type unescape_rules,
                              std::wstring* output,
                              url_parse::Component* out_component,
                              size_t* offset_for_adjustment) {
  DCHECK(output);
  DCHECK(offset_for_adjustment);
  if (in_component.is_nonempty()) {
    out_component->begin = static_cast<int>(output->length());
    size_t offset_past_current_output =
        ((*offset_for_adjustment == std::wstring::npos) ||
         (*offset_for_adjustment < output->length())) ?
            std::wstring::npos : (*offset_for_adjustment - output->length());
    size_t* offset_into_component =
        (offset_past_current_output >= static_cast<size_t>(in_component.len)) ?
            NULL : &offset_past_current_output;
    if (unescape_rules == UnescapeRule::NONE) {
      output->append(UTF8ToWideAndAdjustOffset(
          spec.substr(in_component.begin, in_component.len),
          offset_into_component));
    } else {
      output->append(UTF16ToWideHack(UnescapeAndDecodeUTF8URLComponent(
          spec.substr(in_component.begin, in_component.len), unescape_rules,
          offset_into_component)));
    }
    out_component->len =
        static_cast<int>(output->length()) - out_component->begin;
    if (offset_into_component) {
      *offset_for_adjustment = (*offset_into_component == std::wstring::npos) ?
          std::wstring::npos : (out_component->begin + *offset_into_component);
    } else if (offset_past_current_output != std::wstring::npos) {
      *offset_for_adjustment += out_component->len - in_component.len;
    }
  } else {
    out_component->reset();
  }
}
