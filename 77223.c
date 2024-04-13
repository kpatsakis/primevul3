void GraphicsContext3DPrivate::createGraphicsSurfaces(const IntSize& size)
{
#if USE(GRAPHICS_SURFACE)
    if (size.isEmpty())
        m_graphicsSurface.clear();
    else
        m_graphicsSurface = GraphicsSurface::create(size, m_surfaceFlags, m_platformContext);
#endif
}
