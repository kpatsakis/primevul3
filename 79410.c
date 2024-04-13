void ImageLoader::dispatchPendingEvent(ImageEventSender* eventSender)
{
    ASSERT(eventSender == &beforeLoadEventSender() || eventSender == &loadEventSender() || eventSender == &errorEventSender());
    const AtomicString& eventType = eventSender->eventType();
    if (eventType == eventNames().beforeloadEvent)
        dispatchPendingBeforeLoadEvent();
    if (eventType == eventNames().loadEvent)
        dispatchPendingLoadEvent();
    if (eventType == eventNames().errorEvent)
        dispatchPendingErrorEvent();
}
