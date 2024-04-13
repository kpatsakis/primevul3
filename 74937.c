bool RenderView::handleCurrentKeyboardEvent() {
  if (edit_commands_.empty())
    return false;

  WebFrame* frame = webview()->focusedFrame();
  if (!frame)
    return false;

  EditCommands::iterator it = edit_commands_.begin();
  EditCommands::iterator end = edit_commands_.end();

  bool did_execute_command = false;
  for (; it != end; ++it) {
    if (!frame->executeCommand(WebString::fromUTF8(it->name),
                               WebString::fromUTF8(it->value)))
      break;
    did_execute_command = true;
  }

  return did_execute_command;
}
