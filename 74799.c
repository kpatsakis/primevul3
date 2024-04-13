void RenderView::OnFileChooserResponse(const std::vector<FilePath>& paths) {
  if (file_chooser_completions_.empty())
    return;

  WebVector<WebString> ws_file_names(paths.size());
  for (size_t i = 0; i < paths.size(); ++i)
    ws_file_names[i] = webkit_glue::FilePathToWebString(paths[i]);

  if (file_chooser_completions_.front()->completion)
    file_chooser_completions_.front()->completion->didChooseFile(ws_file_names);
  file_chooser_completions_.pop_front();

  if (!file_chooser_completions_.empty()) {
    Send(new ViewHostMsg_RunFileChooser(routing_id_,
        file_chooser_completions_.front()->params));
  }
}
