void RenderFrameImpl::didAddMessageToConsole(
    const blink::WebConsoleMessage& message,
    const blink::WebString& source_name,
    unsigned source_line,
    const blink::WebString& stack_trace) {
  logging::LogSeverity log_severity = logging::LOG_VERBOSE;
  switch (message.level) {
    case blink::WebConsoleMessage::LevelDebug:
      log_severity = logging::LOG_VERBOSE;
      break;
    case blink::WebConsoleMessage::LevelLog:
    case blink::WebConsoleMessage::LevelInfo:
      log_severity = logging::LOG_INFO;
      break;
    case blink::WebConsoleMessage::LevelWarning:
      log_severity = logging::LOG_WARNING;
      break;
    case blink::WebConsoleMessage::LevelError:
      log_severity = logging::LOG_ERROR;
      break;
    default:
      NOTREACHED();
  }

  if (shouldReportDetailedMessageForSource(source_name)) {
    FOR_EACH_OBSERVER(
        RenderViewObserver, render_view_->observers(),
        DetailedConsoleMessageAdded(message.text,
                                    source_name,
                                    stack_trace,
                                    source_line,
                                    static_cast<int32>(log_severity)));
    FOR_EACH_OBSERVER(
        RenderFrameObserver, observers_,
        DetailedConsoleMessageAdded(message.text,
                                    source_name,
                                    stack_trace,
                                    source_line,
                                    static_cast<int32>(log_severity)));
  }

  Send(new FrameHostMsg_AddMessageToConsole(routing_id_,
                                            static_cast<int32>(log_severity),
                                            message.text,
                                            static_cast<int32>(source_line),
                                            source_name));
}
