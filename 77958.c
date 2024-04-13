  GURL GetHrefURL() {
    return ui_test_utils::GetTestUrl(
      base::FilePath(kTestDir),
      base::FilePath(FILE_PATH_LITERAL("href.html")));
  }
