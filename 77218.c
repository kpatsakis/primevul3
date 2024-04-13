GraphicsContext3DPrivate::GraphicsContext3DPrivate(GraphicsContext3D* context, HostWindow* hostWindow, GraphicsContext3D::RenderStyle renderStyle)
    : m_context(context)
    , m_hostWindow(hostWindow)
    , m_surface(0)
    , m_platformContext(0)
    , m_surfaceOwner(0)
{
    if (renderStyle == GraphicsContext3D::RenderToCurrentGLContext) {
        m_platformContext = QOpenGLContext::currentContext();
        m_surface = m_platformContext->surface();
        return;
    }

#if USE(GRAPHICS_SURFACE)
    QWindow* window = new QWindow;
    window->setSurfaceType(QSurface::OpenGLSurface);
    window->setGeometry(-10, -10, 1, 1);
    window->create();
    m_surface = window;
    m_surfaceOwner = window;

    m_platformContext = new QOpenGLContext(window);
    if (!m_platformContext->create())
        return;

    makeCurrentIfNeeded();
    IntSize surfaceSize(m_context->m_currentWidth, m_context->m_currentHeight);
    m_surfaceFlags = GraphicsSurface::SupportsTextureTarget
                    | GraphicsSurface::SupportsSharing;

    if (!surfaceSize.isEmpty())
        m_graphicsSurface = GraphicsSurface::create(surfaceSize, m_surfaceFlags, m_platformContext);
#endif
}
