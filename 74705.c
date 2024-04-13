AutoFillHelper::AutoFillHelper(RenderView* render_view)
    : render_view_(render_view),
      autofill_query_id_(0),
      autofill_action_(AUTOFILL_NONE),
      suggestions_clear_index_(-1),
      suggestions_options_index_(-1) {
}
