void ImageLoader::dispatchPendingBeforeLoadEvents()
{
    beforeLoadEventSender().dispatchPendingEvents();
}
