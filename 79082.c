net::RequestPriority ConvertWebKitPriorityToNetPriority(
    const WebURLRequest::Priority& priority) {
  switch (priority) {
    case WebURLRequest::PriorityVeryHigh:
      return net::HIGHEST;

    case WebURLRequest::PriorityHigh:
      return net::MEDIUM;

    case WebURLRequest::PriorityMedium:
      return net::LOW;

    case WebURLRequest::PriorityLow:
      return net::LOWEST;

    case WebURLRequest::PriorityVeryLow:
      return net::IDLE;

    case WebURLRequest::PriorityUnresolved:
    default:
      NOTREACHED();
      return net::LOW;
  }
}
