void CopyOneComponent(const char* source,
                      const url_parse::Component& source_component,
                      CanonOutput* output,
                      url_parse::Component* output_component) {
  if (source_component.len < 0) {
    *output_component = url_parse::Component();
    return;
  }

  output_component->begin = output->length();
  int source_end = source_component.end();
  for (int i = source_component.begin; i < source_end; i++)
    output->push_back(source[i]);
  output_component->len = output->length() - output_component->begin;
}
