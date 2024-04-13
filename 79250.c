bool WindowsUpdateFunction::RunImpl() {
  int window_id = extension_misc::kUnknownWindowId;
  EXTENSION_FUNCTION_VALIDATE(args_->GetInteger(0, &window_id));
  DictionaryValue* update_props;
  EXTENSION_FUNCTION_VALIDATE(args_->GetDictionary(1, &update_props));

  WindowController* controller;
  if (!GetWindowFromWindowID(this, window_id, &controller))
    return false;

#if defined(OS_WIN)
  if (win8::IsSingleWindowMetroMode()) {
    SetResult(controller->CreateWindowValue());
    return true;
  }
#endif

  ui::WindowShowState show_state = ui::SHOW_STATE_DEFAULT;  // No change.
  std::string state_str;
  if (update_props->HasKey(keys::kShowStateKey)) {
    EXTENSION_FUNCTION_VALIDATE(update_props->GetString(keys::kShowStateKey,
                                                        &state_str));
    if (state_str == keys::kShowStateValueNormal) {
      show_state = ui::SHOW_STATE_NORMAL;
    } else if (state_str == keys::kShowStateValueMinimized) {
      show_state = ui::SHOW_STATE_MINIMIZED;
    } else if (state_str == keys::kShowStateValueMaximized) {
      show_state = ui::SHOW_STATE_MAXIMIZED;
    } else if (state_str == keys::kShowStateValueFullscreen) {
      show_state = ui::SHOW_STATE_FULLSCREEN;
    } else {
      error_ = keys::kInvalidWindowStateError;
      return false;
    }
  }

  if (show_state != ui::SHOW_STATE_FULLSCREEN &&
      show_state != ui::SHOW_STATE_DEFAULT)
    controller->SetFullscreenMode(false, GetExtension()->url());

  switch (show_state) {
    case ui::SHOW_STATE_MINIMIZED:
      controller->window()->Minimize();
      break;
    case ui::SHOW_STATE_MAXIMIZED:
      controller->window()->Maximize();
      break;
    case ui::SHOW_STATE_FULLSCREEN:
      if (controller->window()->IsMinimized() ||
          controller->window()->IsMaximized())
        controller->window()->Restore();
      controller->SetFullscreenMode(true, GetExtension()->url());
      break;
    case ui::SHOW_STATE_NORMAL:
      controller->window()->Restore();
      break;
    default:
      break;
  }

  gfx::Rect bounds;
  if (controller->window()->IsMinimized())
    bounds = controller->window()->GetRestoredBounds();
  else
    bounds = controller->window()->GetBounds();
  bool set_bounds = false;

  int bounds_val;
  if (update_props->HasKey(keys::kLeftKey)) {
    EXTENSION_FUNCTION_VALIDATE(update_props->GetInteger(
        keys::kLeftKey,
        &bounds_val));
    bounds.set_x(bounds_val);
    set_bounds = true;
  }

  if (update_props->HasKey(keys::kTopKey)) {
    EXTENSION_FUNCTION_VALIDATE(update_props->GetInteger(
        keys::kTopKey,
        &bounds_val));
    bounds.set_y(bounds_val);
    set_bounds = true;
  }

  if (update_props->HasKey(keys::kWidthKey)) {
    EXTENSION_FUNCTION_VALIDATE(update_props->GetInteger(
        keys::kWidthKey,
        &bounds_val));
    bounds.set_width(bounds_val);
    set_bounds = true;
  }

  if (update_props->HasKey(keys::kHeightKey)) {
    EXTENSION_FUNCTION_VALIDATE(update_props->GetInteger(
        keys::kHeightKey,
        &bounds_val));
    bounds.set_height(bounds_val);
    set_bounds = true;
  }

  if (set_bounds) {
    if (show_state == ui::SHOW_STATE_MINIMIZED ||
        show_state == ui::SHOW_STATE_MAXIMIZED ||
        show_state == ui::SHOW_STATE_FULLSCREEN) {
      error_ = keys::kInvalidWindowStateError;
      return false;
    }
    controller->window()->SetBounds(bounds);
  }

  bool active_val = false;
  if (update_props->HasKey(keys::kFocusedKey)) {
    EXTENSION_FUNCTION_VALIDATE(update_props->GetBoolean(
        keys::kFocusedKey, &active_val));
    if (active_val) {
      if (show_state == ui::SHOW_STATE_MINIMIZED) {
        error_ = keys::kInvalidWindowStateError;
        return false;
      }
      controller->window()->Activate();
    } else {
      if (show_state == ui::SHOW_STATE_MAXIMIZED ||
          show_state == ui::SHOW_STATE_FULLSCREEN) {
        error_ = keys::kInvalidWindowStateError;
        return false;
      }
      controller->window()->Deactivate();
    }
  }

  bool draw_attention = false;
  if (update_props->HasKey(keys::kDrawAttentionKey)) {
    EXTENSION_FUNCTION_VALIDATE(update_props->GetBoolean(
        keys::kDrawAttentionKey, &draw_attention));
    controller->window()->FlashFrame(draw_attention);
  }

  SetResult(controller->CreateWindowValue());

  return true;
}
