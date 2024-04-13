bool Extension::HasFullPermissions() const {
  return !plugins().empty();
}
