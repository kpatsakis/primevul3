bool CreateSymbolicLink(const FilePath& target_path,
                        const FilePath& symlink_path) {
  DCHECK(!symlink_path.empty());
  DCHECK(!target_path.empty());
  return ::symlink(target_path.value().c_str(),
                   symlink_path.value().c_str()) != -1;
}
