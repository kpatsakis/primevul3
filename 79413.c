void ImageLoader::elementDidMoveToNewDocument()
{
    clearFailedLoadURL();
    setImage(0);
}
