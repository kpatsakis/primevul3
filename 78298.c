OmniboxEditModel::OmniboxEditModel(OmniboxView* view,
                                   OmniboxEditController* controller,
                                   Profile* profile)
    : view_(view),
      controller_(controller),
      focus_state_(OMNIBOX_FOCUS_NONE),
      focus_source_(INVALID),
      user_input_in_progress_(false),
      user_input_since_focus_(true),
      just_deleted_text_(false),
      has_temporary_text_(false),
      paste_state_(NONE),
      control_key_state_(UP),
      is_keyword_hint_(false),
      profile_(profile),
      in_revert_(false),
      allow_exact_keyword_match_(false) {
  omnibox_controller_.reset(new OmniboxController(this, profile));
  delegate_.reset(new OmniboxCurrentPageDelegateImpl(controller, profile));
}
