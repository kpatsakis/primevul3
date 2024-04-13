void GraphicsContext3DPrivate::blitMultisampleFramebufferAndRestoreContext() const
{
    if (!m_context->m_attrs.antialias)
        return;

    const QOpenGLContext* currentContext = QOpenGLContext::currentContext();
    QSurface* currentSurface = 0;
    if (currentContext && currentContext != m_platformContext) {
        currentSurface = currentContext->surface();
        m_platformContext->makeCurrent(m_surface);
    }
    blitMultisampleFramebuffer();
    if (currentContext && currentContext != m_platformContext)
        const_cast<QOpenGLContext*>(currentContext)->makeCurrent(currentSurface);
}
