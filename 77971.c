  void RunTest(Browser* browser,
               const GURL& url,
               int modifiers,
               WebKit::WebMouseEvent::Button button,
               WindowOpenDisposition disposition) {
    ui_test_utils::NavigateToURL(browser, url);
    EXPECT_EQ(1u, chrome::GetBrowserCount(browser->profile(),
                                          browser->host_desktop_type()));
    EXPECT_EQ(1, browser->tab_strip_model()->count());
    content::WebContents* web_contents =
        browser->tab_strip_model()->GetActiveWebContents();
    EXPECT_EQ(url, web_contents->GetURL());

    if (disposition == CURRENT_TAB) {
      content::WebContents* web_contents =
          browser->tab_strip_model()->GetActiveWebContents();
      content::TestNavigationObserver same_tab_observer(web_contents);
      SimulateMouseClick(web_contents, modifiers, button);
      base::RunLoop run_loop;
      same_tab_observer.WaitForObservation(
          base::Bind(&content::RunThisRunLoop, base::Unretained(&run_loop)),
          content::GetQuitTaskForRunLoop(&run_loop));
      EXPECT_EQ(1u, chrome::GetBrowserCount(browser->profile(),
                                            browser->host_desktop_type()));
      EXPECT_EQ(1, browser->tab_strip_model()->count());
      EXPECT_EQ(getSecondPageTitle(), web_contents->GetTitle());
      return;
    }

    content::WindowedNotificationObserver observer(
        chrome::NOTIFICATION_TAB_ADDED,
        content::NotificationService::AllSources());
    SimulateMouseClick(web_contents, modifiers, button);
    observer.Wait();

    if (disposition == NEW_WINDOW) {
      EXPECT_EQ(2u, chrome::GetBrowserCount(browser->profile(),
                                            browser->host_desktop_type()));
      return;
    }

    EXPECT_EQ(1u, chrome::GetBrowserCount(browser->profile(),
                                          browser->host_desktop_type()));
    EXPECT_EQ(2, browser->tab_strip_model()->count());
    web_contents = browser->tab_strip_model()->GetActiveWebContents();
    WaitForLoadStop(web_contents);
    if (disposition == NEW_FOREGROUND_TAB) {
      EXPECT_EQ(getSecondPageTitle(), web_contents->GetTitle());
    } else {
      ASSERT_EQ(NEW_BACKGROUND_TAB, disposition);
      EXPECT_EQ(getFirstPageTitle(), web_contents->GetTitle());
    }
  }
