  void RunBeforeUnloadSanityTest(DevToolsDockSide dock_side,
                             base::Callback<void(void)> close_method,
                             bool wait_for_browser_close = true) {
    OpenDevToolsWindow(kDebuggerTestPage);
    window_->SetDockSideForTest(dock_side);
    content::WindowedNotificationObserver devtools_close_observer(
        content::NOTIFICATION_WEB_CONTENTS_DESTROYED,
        content::Source<content::WebContents>(window_->web_contents()));
    InjectBeforeUnloadListener(window_->web_contents());
    {
      DevToolsWindowBeforeUnloadObserver before_unload_observer(window_);
      close_method.Run();
      CancelModalDialog();
      before_unload_observer.Wait();
    }
    {
      content::WindowedNotificationObserver close_observer(
          chrome::NOTIFICATION_BROWSER_CLOSED,
          content::Source<Browser>(browser()));
      close_method.Run();
      AcceptModalDialog();
      if (wait_for_browser_close)
        close_observer.Wait();
    }
    devtools_close_observer.Wait();
  }
