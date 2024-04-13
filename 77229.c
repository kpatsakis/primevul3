bool GraphicsContext3DPrivate::makeCurrentIfNeeded() const
{
    const QOpenGLContext* currentContext = QOpenGLContext::currentContext();
    if (currentContext == m_platformContext)
        return true;
    return m_platformContext->makeCurrent(m_surface);
}
