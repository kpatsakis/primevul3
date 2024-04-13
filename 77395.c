void GraphicsContext::platformInit(PlatformGraphicsContext* gc)
{
    if (gc)
        gc->setGraphicsContext(this);

    m_data = gc;
    setPaintingDisabled(!gc || !gc->canvas());
}
