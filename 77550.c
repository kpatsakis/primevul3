HistoryQuickProvider::HistoryQuickProvider(ACProviderListener* listener,
                                           Profile* profile)
    : HistoryProvider(listener, profile, "HistoryQuickProvider"),
      languages_(profile_->GetPrefs()->GetString(prefs::kAcceptLanguages)) {
  enum InliningOption {
    INLINING_PROHIBITED = 0,
    INLINING_ALLOWED = 1,
    INLINING_AUTO_BUT_NOT_IN_FIELD_TRIAL = 2,
    INLINING_FIELD_TRIAL_DEFAULT_GROUP = 3,
    INLINING_FIELD_TRIAL_EXPERIMENT_GROUP = 4,
    NUM_OPTIONS = 5
  };
  InliningOption inlining_option = NUM_OPTIONS;

  const std::string switch_value = CommandLine::ForCurrentProcess()->
      GetSwitchValueASCII(switches::kOmniboxInlineHistoryQuickProvider);
  if (switch_value == switches::kOmniboxInlineHistoryQuickProviderAllowed) {
    inlining_option = INLINING_ALLOWED;
    always_prevent_inline_autocomplete_ = false;
  } else if (switch_value ==
             switches::kOmniboxInlineHistoryQuickProviderProhibited) {
    inlining_option = INLINING_PROHIBITED;
    always_prevent_inline_autocomplete_ = true;
  } else {

    DCHECK(!content::BrowserThread::IsWellKnownThread(
               content::BrowserThread::UI) ||
           content::BrowserThread::CurrentlyOn(content::BrowserThread::UI));
    if (AutocompleteFieldTrial::InDisallowInlineHQPFieldTrial()) {
      if (AutocompleteFieldTrial::
          InDisallowInlineHQPFieldTrialExperimentGroup()) {
        always_prevent_inline_autocomplete_ = true;
        inlining_option = INLINING_FIELD_TRIAL_EXPERIMENT_GROUP;
      } else {
        always_prevent_inline_autocomplete_ = false;
        inlining_option = INLINING_FIELD_TRIAL_DEFAULT_GROUP;
      }
    } else {
      always_prevent_inline_autocomplete_ = false;
      inlining_option = INLINING_AUTO_BUT_NOT_IN_FIELD_TRIAL;
    }
  }

  UMA_HISTOGRAM_ENUMERATION(
      "Omnibox.InlineHistoryQuickProviderFieldTrialBeacon",
      inlining_option, NUM_OPTIONS);
}
