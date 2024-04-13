void ChangePosixFilePermissions(const FilePath& path,
                                int mode_bits_to_set,
                                int mode_bits_to_clear) {
  ASSERT_FALSE(mode_bits_to_set & mode_bits_to_clear)
      << "Can't set and clear the same bits.";

  int mode = 0;
  ASSERT_TRUE(file_util::GetPosixFilePermissions(path, &mode));
  mode |= mode_bits_to_set;
  mode &= ~mode_bits_to_clear;
  ASSERT_TRUE(file_util::SetPosixFilePermissions(path, mode));
}
