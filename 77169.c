  virtual void SetUpCommandLine(CommandLine* command_line) {
    EnableDOMAutomation();

    InProcessBrowserTest::SetUpCommandLine(command_line);

    if (!command_line->HasSwitch("enable-gpu")) {
#if !defined(OS_MACOSX)
      CHECK(test_launcher_utils::OverrideGLImplementation(
          command_line, gfx::kGLImplementationOSMesaName)) <<
          "kUseGL must not be set by test framework code!";
#endif
    }
    command_line->AppendSwitch(switches::kDisablePopupBlocking);
  }
