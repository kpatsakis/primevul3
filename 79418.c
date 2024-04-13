void ImageLoader::setImage(CachedImage* newImage)
{
    setImageWithoutConsideringPendingLoadEvent(newImage);

    updatedHasPendingEvent();
}
