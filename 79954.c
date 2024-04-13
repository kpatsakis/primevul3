  explicit FindResultCollector(file_util::FileEnumerator& enumerator) {
    FilePath cur_file;
    while (!(cur_file = enumerator.Next()).value().empty()) {
      FilePath::StringType path = cur_file.value();
      EXPECT_TRUE(files_.end() == files_.find(path))
          << "Same file returned twice";

      files_.insert(path);
    }
  }
