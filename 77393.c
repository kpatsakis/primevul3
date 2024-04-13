PlatformGraphicsContext* GraphicsContext::platformContext() const
{
    ASSERT(!paintingDisabled());
    return m_data;
}
