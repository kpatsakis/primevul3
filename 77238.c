GraphicsContext3DPrivate::~GraphicsContext3DPrivate()
{
    delete m_surfaceOwner;
    m_surfaceOwner = 0;
}
