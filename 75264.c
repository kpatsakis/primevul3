void WebGLObject::onDetached(GraphicsContext3D* context3d)
{
    if (m_attachmentCount)
        --m_attachmentCount;
    if (m_deleted)
        deleteObject(context3d);
}
