void WebGLObject::deleteObject(GraphicsContext3D* context3d)
{
    m_deleted = true;
    if (!m_object)
        return;

    if (!hasGroupOrContext())
        return;

    if (!m_attachmentCount) {
        if (!context3d)
            context3d = getAGraphicsContext3D();

        if (context3d)
            deleteObjectImpl(context3d, m_object);

        m_object = 0;
    }
}
