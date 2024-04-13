GraphicsContext3D::~GraphicsContext3D()
{
    if (!m_private)
        return;

    makeContextCurrent();
    glDeleteTextures(1, &m_texture);
    glDeleteFramebuffers(1, &m_fbo);
    if (m_attrs.antialias) {
        glDeleteRenderbuffers(1, &m_multisampleColorBuffer);
        glDeleteFramebuffers(1, &m_multisampleFBO);
        if (m_attrs.stencil || m_attrs.depth)
            glDeleteRenderbuffers(1, &m_multisampleDepthStencilBuffer);
    } else if (m_attrs.stencil || m_attrs.depth) {
#if USE(OPENGL_ES_2)
        if (m_attrs.depth)
            glDeleteRenderbuffers(1, &m_depthBuffer);
        if (m_attrs.stencil)
            glDeleteRenderbuffers(1, &m_stencilBuffer);
#endif
        glDeleteRenderbuffers(1, &m_depthStencilBuffer);
    }
}
