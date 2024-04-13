IntSize GraphicsContext3DPrivate::platformLayerSize() const
{
    return IntSize(m_context->m_currentWidth, m_context->m_currentHeight);
}
