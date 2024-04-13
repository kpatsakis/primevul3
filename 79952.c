void CreateTextFile(const FilePath& filename,
                    const std::wstring& contents) {
  std::wofstream file;
  file.open(filename.value().c_str());
  ASSERT_TRUE(file.is_open());
  file << contents;
  file.close();
}
