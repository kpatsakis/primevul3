void URLFixerUpper::OffsetComponent(int offset, url_parse::Component* part) {
  DCHECK(part);

  if (part->is_valid()) {
    part->begin += offset;

    if (part->begin < 0)
      part->reset();
  }
}
