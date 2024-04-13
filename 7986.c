  static void MapToLabels(const string& subscript, Labels* labels,
                          absl::flat_hash_map<char, int>* label_mapping) {
    for (int i = 0; i < subscript.size(); ++i) {
      const char label_char = subscript[i];
      if (label_char == '.') {
        labels->push_back(kEllipsisLabel);
        i += 2;  // Skip next 2 characters as well.
        continue;
      }
      if (!label_mapping->contains(label_char)) {
        const int next_label = label_mapping->size();
        (*label_mapping)[label_char] = next_label;
      }
      const int mapped_label = (*label_mapping)[label_char];
      labels->push_back(mapped_label);
    }
  }