OmniboxView* OmniboxView::CreateOmniboxView(
    AutocompleteEditController* controller,
    ToolbarModel* toolbar_model,
    Profile* profile,
    CommandUpdater* command_updater,
    bool popup_window_mode,
    LocationBarView* location_bar) {
  if (views::Widget::IsPureViews()) {
    OmniboxViewViews* omnibox_view = new OmniboxViewViews(controller,
                                                          toolbar_model,
                                                          profile,
                                                          command_updater,
                                                          popup_window_mode,
                                                          location_bar);
    omnibox_view->Init();
    return omnibox_view;
  }
  return new OmniboxViewWin(controller,
                            toolbar_model,
                            location_bar,
                            command_updater,
                            popup_window_mode,
                            location_bar);
}
