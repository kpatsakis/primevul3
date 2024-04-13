GraphicsContext3D::GraphicsContext3D(GraphicsContext3D::Attributes attrs, HostWindow* hostWindow, GraphicsContext3D::RenderStyle renderStyle)
    : m_currentWidth(0)
    , m_currentHeight(0)
    , m_compiler(isGLES2Compliant() ? SH_ESSL_OUTPUT : SH_GLSL_OUTPUT)
    , m_attrs(attrs)
    , m_renderStyle(renderStyle)
    , m_texture(0)
    , m_compositorTexture(0)
    , m_fbo(0)
#if USE(OPENGL_ES_2)
    , m_depthBuffer(0)
    , m_stencilBuffer(0)
#endif
    , m_depthStencilBuffer(0)
    , m_layerComposited(false)
    , m_internalColorFormat(0)
    , m_boundFBO(0)
    , m_activeTexture(GL_TEXTURE0)
    , m_boundTexture0(0)
    , m_multisampleFBO(0)
    , m_multisampleDepthStencilBuffer(0)
    , m_multisampleColorBuffer(0)
    , m_private(adoptPtr(new GraphicsContext3DPrivate(this, hostWindow, renderStyle)))
{
    validateAttributes();

    if (!m_private->m_surface) {
        LOG_ERROR("GraphicsContext3D: QGLWidget initialization failed.");
        m_private = nullptr;
        return;
    }

    static bool initialized = false;
    static bool success = true;
    if (!initialized) {
        success = initializeOpenGLShims();
        initialized = true;
    }
    if (!success) {
        m_private = nullptr;
        return;
    }

    if (renderStyle == RenderOffscreen)
        m_private->createOffscreenBuffers();

    m_private->initializeANGLE();

#if !USE(OPENGL_ES_2)
    glEnable(GL_POINT_SPRITE);
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
#endif

    if (renderStyle != RenderToCurrentGLContext)
        glClearColor(0.0, 0.0, 0.0, 0.0);
}
