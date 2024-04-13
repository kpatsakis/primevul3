  virtual void SetUpInProcessBrowserTestFixture() {
    FilePath test_dir;
    ASSERT_TRUE(PathService::Get(chrome::DIR_TEST_DATA, &test_dir));
    gpu_test_dir_ = test_dir.AppendASCII("gpu");
  }
