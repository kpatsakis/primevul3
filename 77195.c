void Clipboard::InsertMapping(const char* key,
                              char* data,
                              size_t data_len) {
  DCHECK(clipboard_data_->find(key) == clipboard_data_->end());
  (*clipboard_data_)[key] = std::make_pair(data, data_len);
}
