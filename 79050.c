int HttpStreamParser::DoLoop(int result) {
  bool can_do_more = true;
  do {
    switch (io_state_) {
      case STATE_SENDING_HEADERS:
        if (result < 0)
          can_do_more = false;
        else
          result = DoSendHeaders(result);
        break;
      case STATE_SENDING_BODY:
        if (result < 0)
          can_do_more = false;
        else
          result = DoSendBody(result);
        break;
      case STATE_SEND_REQUEST_READING_BODY:
        result = DoSendRequestReadingBody(result);
        break;
      case STATE_REQUEST_SENT:
        DCHECK(result != ERR_IO_PENDING);
        can_do_more = false;
        break;
      case STATE_READ_HEADERS:
        net_log_.BeginEvent(NetLog::TYPE_HTTP_STREAM_PARSER_READ_HEADERS);
        result = DoReadHeaders();
        break;
      case STATE_READ_HEADERS_COMPLETE:
        result = DoReadHeadersComplete(result);
        net_log_.EndEventWithNetErrorCode(
            NetLog::TYPE_HTTP_STREAM_PARSER_READ_HEADERS, result);
        break;
      case STATE_BODY_PENDING:
        DCHECK(result != ERR_IO_PENDING);
        can_do_more = false;
        break;
      case STATE_READ_BODY:
        result = DoReadBody();
        break;
      case STATE_READ_BODY_COMPLETE:
        result = DoReadBodyComplete(result);
        break;
      case STATE_DONE:
        DCHECK(result != ERR_IO_PENDING);
        can_do_more = false;
        break;
      default:
        NOTREACHED();
        can_do_more = false;
        break;
    }
  } while (result != ERR_IO_PENDING && can_do_more);

  return result;
}
