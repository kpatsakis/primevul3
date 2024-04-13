void ImageLoader::updateRenderer()
{
    RenderImageResource* imageResource = renderImageResource();

    if (!imageResource)
        return;

    CachedImage* cachedImage = imageResource->cachedImage();
    if (m_image != cachedImage && (m_imageComplete || !cachedImage))
        imageResource->setCachedImage(m_image.get());
}
