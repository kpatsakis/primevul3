void GraphicsContext3DPrivate::createOffscreenBuffers()
{
    glGenFramebuffers(/* count */ 1, &m_context->m_fbo);

    glGenTextures(1, &m_context->m_texture);
    glBindTexture(GraphicsContext3D::TEXTURE_2D, m_context->m_texture);
    glTexParameterf(GraphicsContext3D::TEXTURE_2D, GraphicsContext3D::TEXTURE_MAG_FILTER, GraphicsContext3D::LINEAR);
    glTexParameterf(GraphicsContext3D::TEXTURE_2D, GraphicsContext3D::TEXTURE_MIN_FILTER, GraphicsContext3D::LINEAR);
    glTexParameteri(GraphicsContext3D::TEXTURE_2D, GraphicsContext3D::TEXTURE_WRAP_S, GraphicsContext3D::CLAMP_TO_EDGE);
    glTexParameteri(GraphicsContext3D::TEXTURE_2D, GraphicsContext3D::TEXTURE_WRAP_T, GraphicsContext3D::CLAMP_TO_EDGE);
    glBindTexture(GraphicsContext3D::TEXTURE_2D, 0);

    if (m_context->m_attrs.antialias) {
        glGenFramebuffers(1, &m_context->m_multisampleFBO);
        glBindFramebuffer(GraphicsContext3D::FRAMEBUFFER, m_context->m_multisampleFBO);
        m_context->m_boundFBO = m_context->m_multisampleFBO;
        glGenRenderbuffers(1, &m_context->m_multisampleColorBuffer);
        if (m_context->m_attrs.stencil || m_context->m_attrs.depth)
            glGenRenderbuffers(1, &m_context->m_multisampleDepthStencilBuffer);
    } else {
        glBindFramebuffer(GraphicsContext3D::FRAMEBUFFER, m_context->m_fbo);
        m_context->m_boundFBO = m_context->m_fbo;
#if USE(OPENGL_ES_2)
        if (m_context->m_attrs.depth)
            glGenRenderbuffers(1, &m_context->m_depthBuffer);
        if (m_context->m_attrs.stencil)
            glGenRenderbuffers(1, &m_context->m_stencilBuffer);
#endif
        if (m_context->m_attrs.stencil || m_context->m_attrs.depth)
            glGenRenderbuffers(1, &m_context->m_depthStencilBuffer);
    }
}
