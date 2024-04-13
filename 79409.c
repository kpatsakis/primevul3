void ImageLoader::dispatchPendingErrorEvents()
{
    errorEventSender().dispatchPendingEvents();
}
