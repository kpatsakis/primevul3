int LayoutTestBrowserMain(
    const content::MainFunctionParams& parameters,
    const scoped_ptr<content::BrowserMainRunner>& main_runner) {
  base::ScopedTempDir browser_context_path_for_layout_tests;

  CHECK(browser_context_path_for_layout_tests.CreateUniqueTempDir());
  CHECK(!browser_context_path_for_layout_tests.path().MaybeAsASCII().empty());
  CommandLine::ForCurrentProcess()->AppendSwitchASCII(
      switches::kContentShellDataPath,
      browser_context_path_for_layout_tests.path().MaybeAsASCII());

#if defined(OS_ANDROID)
  content::EnsureInitializeForAndroidLayoutTests();
#endif

  int exit_code = main_runner->Initialize(parameters);
  DCHECK_LT(exit_code, 0)
      << "BrowserMainRunner::Initialize failed in LayoutTestBrowserMain";

  if (exit_code >= 0)
    return exit_code;

  if (CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kCheckLayoutTestSysDeps)) {
    base::MessageLoop::current()->PostTask(FROM_HERE,
                                           base::MessageLoop::QuitClosure());
    main_runner->Run();
    content::Shell::CloseAllWindows();
    main_runner->Shutdown();
    return 0;
  }

  content::WebKitTestController test_controller;
  {
    base::ThreadRestrictions::ScopedAllowIO allow_io;
    base::FilePath temp_path;
    base::GetTempDir(&temp_path);
    test_controller.SetTempPath(temp_path);
  }
  std::string test_string;
  CommandLine::StringVector args = CommandLine::ForCurrentProcess()->GetArgs();
  size_t command_line_position = 0;
  bool ran_at_least_once = false;

  std::cout << "#READY\n";
  std::cout.flush();

  while (GetNextTest(args, &command_line_position, &test_string)) {
    if (!RunOneTest(test_string, &ran_at_least_once, main_runner))
      break;
  }
  if (!ran_at_least_once) {
    base::MessageLoop::current()->PostTask(FROM_HERE,
                                           base::MessageLoop::QuitClosure());
    main_runner->Run();
  }

#if defined(OS_ANDROID)
  main_runner->Shutdown();
#endif

  exit_code = 0;

#if !defined(OS_ANDROID)
  main_runner->Shutdown();
#endif

  return exit_code;
}
