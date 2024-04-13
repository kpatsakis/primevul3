void HttpStreamParser::OnIOComplete(int result) {
  result = DoLoop(result);

  if (result != ERR_IO_PENDING && !callback_.is_null()) {
    CompletionCallback c = callback_;
    callback_.Reset();
    c.Run(result);
  }
}
