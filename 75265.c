void WebGLObject::setObject(Platform3DObject object)
{
    ASSERT(!m_object && !m_deleted);
    m_object = object;
}
