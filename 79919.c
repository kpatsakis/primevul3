FileEnumerator::FileEnumerator(const FilePath& root_path,
                               bool recursive,
                               int file_type)
    : current_directory_entry_(0),
      root_path_(root_path),
      recursive_(recursive),
      file_type_(file_type) {
  DCHECK(!(recursive && (INCLUDE_DOT_DOT & file_type_)));
  pending_paths_.push(root_path);
}
