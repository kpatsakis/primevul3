void RenderFrameImpl::willSubmitForm(blink::WebLocalFrame* frame,
                                     const blink::WebFormElement& form) {
  DCHECK(!frame_ || frame_ == frame);
  DocumentState* document_state =
      DocumentState::FromDataSource(frame->provisionalDataSource());
  NavigationState* navigation_state = document_state->navigation_state();
  InternalDocumentStateData* internal_data =
      InternalDocumentStateData::FromDocumentState(document_state);

  if (PageTransitionCoreTypeIs(navigation_state->transition_type(),
                               PAGE_TRANSITION_LINK)) {
    navigation_state->set_transition_type(PAGE_TRANSITION_FORM_SUBMIT);
  }

  WebSearchableFormData web_searchable_form_data(form);
  internal_data->set_searchable_form_url(web_searchable_form_data.url());
  internal_data->set_searchable_form_encoding(
      web_searchable_form_data.encoding().utf8());

  FOR_EACH_OBSERVER(RenderViewObserver, render_view_->observers(),
                    WillSubmitForm(frame, form));
}
