void GraphicsContext3DPrivate::blitMultisampleFramebuffer() const
{
    if (!m_context->m_attrs.antialias)
        return;
#if !USE(OPENGL_ES_2)
    glBindFramebuffer(GL_READ_FRAMEBUFFER_EXT, m_context->m_multisampleFBO);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER_EXT, m_context->m_fbo);
    glBlitFramebuffer(0, 0, m_context->m_currentWidth, m_context->m_currentHeight, 0, 0, m_context->m_currentWidth, m_context->m_currentHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
#endif
    glBindFramebuffer(GraphicsContext3D::FRAMEBUFFER, m_context->m_boundFBO);
}
