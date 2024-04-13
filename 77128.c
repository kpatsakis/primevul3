static std::string ExtractManagerStreamLabel(
    const std::string& track_label) {
  std::string manager_label = track_label;
  size_t pos = manager_label.rfind("#");
  manager_label = manager_label.substr(0, pos);
  return manager_label;
}
