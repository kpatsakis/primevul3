void ImageLoader::timerFired(Timer<ImageLoader>*)
{
    m_element->deref();
}
