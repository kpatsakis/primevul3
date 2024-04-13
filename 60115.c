edge_reason_is_retriable(int reason)
{
  return reason == END_STREAM_REASON_HIBERNATING ||
         reason == END_STREAM_REASON_RESOURCELIMIT ||
         reason == END_STREAM_REASON_EXITPOLICY ||
         reason == END_STREAM_REASON_RESOLVEFAILED ||
         reason == END_STREAM_REASON_MISC ||
         reason == END_STREAM_REASON_NOROUTE;
}
