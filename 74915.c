void RenderView::didFinishDocumentLoad(WebFrame* frame) {
  WebDataSource* ds = frame->dataSource();
  NavigationState* navigation_state = NavigationState::FromDataSource(ds);
  DCHECK(navigation_state);
  navigation_state->set_finish_document_load_time(Time::Now());

  Send(new ViewHostMsg_DocumentLoadedInFrame(routing_id_));

  autofill_helper_.FrameContentsAvailable(frame);
  password_autocomplete_manager_.SendPasswordForms(frame, false);

  UpdateEncoding(frame, frame->view()->pageEncoding().utf8());

  if (RenderThread::current()) {  // Will be NULL during unit tests.
    RenderThread::current()->user_script_slave()->InjectScripts(
        frame, UserScript::DOCUMENT_END);
  }

  navigation_state->user_script_idle_scheduler()->DidFinishDocumentLoad();
}
