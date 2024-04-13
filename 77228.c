bool GraphicsContext3D::makeContextCurrent()
{
    if (!m_private || m_renderStyle == RenderToCurrentGLContext)
        return false;
    return m_private->makeCurrentIfNeeded();
}
