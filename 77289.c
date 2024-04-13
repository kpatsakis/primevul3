OmniboxViewWin::OmniboxViewWin(AutocompleteEditController* controller,
                               ToolbarModel* toolbar_model,
                               LocationBarView* parent_view,
                               CommandUpdater* command_updater,
                               bool popup_window_mode,
                               views::View* location_bar)
    : model_(new AutocompleteEditModel(this, controller,
                                       parent_view->browser()->profile())),
      popup_view_(new AutocompletePopupContentsView(parent_view->font(), this,
                                                    model_.get(),
                                                    location_bar)),
      controller_(controller),
      parent_view_(parent_view),
      toolbar_model_(toolbar_model),
      command_updater_(command_updater),
      popup_window_mode_(popup_window_mode),
      force_hidden_(false),
      tracking_click_(),
      tracking_double_click_(false),
      double_click_time_(0),
      can_discard_mousemove_(false),
      ignore_ime_messages_(false),
      delete_at_end_pressed_(false),
      font_(parent_view->font()),
      possible_drag_(false),
      in_drag_(false),
      initiated_drag_(false),
      drop_highlight_position_(-1),
      ime_candidate_window_open_(false),
      background_color_(skia::SkColorToCOLORREF(LocationBarView::GetColor(
          ToolbarModel::NONE, LocationBarView::BACKGROUND))),
      security_level_(ToolbarModel::NONE),
      text_object_model_(NULL) {
  CreateTextServices(NULL, NULL, NULL);

  saved_selection_for_focus_change_.cpMin = -1;

  g_paint_patcher.Pointer()->RefPatch();

  Create(location_bar->GetWidget()->GetNativeView(), 0, 0, 0,
         l10n_util::GetExtendedStyles());
  SetReadOnly(popup_window_mode_);
  SetFont(font_.GetNativeFont());

  SendMessage(m_hWnd, EM_SETWORDBREAKPROC, 0,
              reinterpret_cast<LPARAM>(&WordBreakProc));

  HDC dc = ::GetDC(NULL);
  SelectObject(dc, font_.GetNativeFont());
  TEXTMETRIC tm = {0};
  GetTextMetrics(dc, &tm);
  const float kXHeightRatio = 0.7f;  // The ratio of a font's x-height to its
  font_x_height_ = static_cast<int>((static_cast<float>(font_.GetBaseline() -
      tm.tmInternalLeading) * kXHeightRatio) + 0.5);
  const int kTextBaseline = popup_window_mode_ ? 15 : 18;
  font_y_adjustment_ = kTextBaseline - font_.GetBaseline();

  const long kTwipsPerPixel = kTwipsPerInch / GetDeviceCaps(dc, LOGPIXELSY);
  ::ReleaseDC(NULL, dc);

  CHARFORMAT cf = {0};
  cf.dwMask = CFM_OFFSET;
  cf.yOffset = -font_y_adjustment_ * kTwipsPerPixel;
  SetDefaultCharFormat(cf);

  SetBackgroundColor(background_color_);

  RevokeDragDrop(m_hWnd);

  if (!popup_window_mode_) {
    scoped_refptr<EditDropTarget> drop_target = new EditDropTarget(this);
    RegisterDragDrop(m_hWnd, drop_target.get());
  }
}
