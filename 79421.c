void ImageLoader::updateFromElementIgnoringPreviousError()
{
    clearFailedLoadURL();
    updateFromElement();
}
