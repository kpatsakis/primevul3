void WebGLObject::detach()
{
    m_attachmentCount = 0; // Make sure OpenGL resource is deleted.
    }
