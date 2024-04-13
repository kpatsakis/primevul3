void ExtensionService::RecordPermissionMessagesHistogram(
    const Extension* e, const char* histogram) {
  base::Histogram* counter = base::LinearHistogram::FactoryGet(
      histogram,
      1,
      ExtensionPermissionMessage::kEnumBoundary,
      ExtensionPermissionMessage::kEnumBoundary + 1,
      base::Histogram::kUmaTargetedHistogramFlag);

  ExtensionPermissionMessages permissions = e->GetPermissionMessages();
  if (permissions.empty()) {
    counter->Add(ExtensionPermissionMessage::kNone);
  } else {
    for (ExtensionPermissionMessages::iterator it = permissions.begin();
         it != permissions.end(); ++it)
      counter->Add(it->id());
  }
}
