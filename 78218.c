bool RunOneTest(const std::string& test_string,
                bool* ran_at_least_once,
                const scoped_ptr<content::BrowserMainRunner>& main_runner) {
  if (test_string.empty())
    return true;
  if (test_string == "QUIT")
    return false;

  bool enable_pixel_dumps;
  std::string pixel_hash;
  base::FilePath cwd;
  GURL test_url =
      GetURLForLayoutTest(test_string, &cwd, &enable_pixel_dumps, &pixel_hash);
  if (!content::WebKitTestController::Get()->PrepareForLayoutTest(
          test_url, cwd, enable_pixel_dumps, pixel_hash)) {
    return false;
  }

  *ran_at_least_once = true;
#if defined(OS_ANDROID)
  base::RunLoop run_loop;
  run_loop.Run();
#else
  main_runner->Run();
#endif

  if (!content::WebKitTestController::Get()->ResetAfterLayoutTest())
    return false;

#if defined(OS_ANDROID)
  base::MessageLoop::current()->RunUntilIdle();
#endif
  return true;
}
