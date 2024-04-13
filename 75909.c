void ChromeContentRendererClient::RenderViewCreated(RenderView* render_view) {
  safe_browsing::PhishingClassifierDelegate* phishing_classifier = NULL;
#ifndef OS_CHROMEOS
  if (!CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kDisableClientSidePhishingDetection)) {
    phishing_classifier =
        safe_browsing::PhishingClassifierDelegate::Create(render_view, NULL);
  }
#endif

  ContentSettingsObserver* content_settings =
      new ContentSettingsObserver(render_view);
  new DevToolsAgent(render_view);
  new ExtensionHelper(render_view, extension_dispatcher_.get());
  new MHTMLGenerator(render_view);
  new PageLoadHistograms(render_view, histogram_snapshots_.get());
  new PrintWebViewHelper(render_view);
  new SearchBox(render_view);
  new SpellCheckProvider(render_view, spellcheck_.get());
  safe_browsing::MalwareDOMDetails::Create(render_view);

#if defined(OS_MACOSX)
  new TextInputClientObserver(render_view);
#endif  // defined(OS_MACOSX)

  PasswordAutofillManager* password_autofill_manager =
      new PasswordAutofillManager(render_view);
  AutofillAgent* autofill_agent = new AutofillAgent(render_view,
                                                    password_autofill_manager);
  PageClickTracker* page_click_tracker = new PageClickTracker(render_view);
  page_click_tracker->AddListener(password_autofill_manager);
  page_click_tracker->AddListener(autofill_agent);

  TranslateHelper* translate = new TranslateHelper(render_view, autofill_agent);
  new ChromeRenderViewObserver(
      render_view, content_settings, extension_dispatcher_.get(),
      translate, phishing_classifier);

  if (CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kDomAutomationController)) {
    new AutomationRendererHelper(render_view);
  }
}
