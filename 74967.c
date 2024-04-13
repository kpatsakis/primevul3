void RenderView::updateSpellingUIWithMisspelledWord(const WebString& word) {
  Send(new ViewHostMsg_UpdateSpellingPanelWithMisspelledWord(routing_id_,
                                                             word));
}
