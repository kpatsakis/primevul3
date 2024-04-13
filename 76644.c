  void RevokeAllPermissionsForFile(const FilePath& file) {
    FilePath stripped = file.StripTrailingSeparators();
    file_permissions_.erase(stripped);
    request_file_set_.erase(stripped);
  }
