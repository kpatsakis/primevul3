void GraphicsContext3DPrivate::initializeANGLE()
{
    ShBuiltInResources ANGLEResources;
    ShInitBuiltInResources(&ANGLEResources);

    m_context->getIntegerv(GraphicsContext3D::MAX_VERTEX_ATTRIBS, &ANGLEResources.MaxVertexAttribs);
    m_context->getIntegerv(GraphicsContext3D::MAX_VERTEX_UNIFORM_VECTORS, &ANGLEResources.MaxVertexUniformVectors);
    m_context->getIntegerv(GraphicsContext3D::MAX_VARYING_VECTORS, &ANGLEResources.MaxVaryingVectors);
    m_context->getIntegerv(GraphicsContext3D::MAX_VERTEX_TEXTURE_IMAGE_UNITS, &ANGLEResources.MaxVertexTextureImageUnits);
    m_context->getIntegerv(GraphicsContext3D::MAX_COMBINED_TEXTURE_IMAGE_UNITS, &ANGLEResources.MaxCombinedTextureImageUnits);
    m_context->getIntegerv(GraphicsContext3D::MAX_TEXTURE_IMAGE_UNITS, &ANGLEResources.MaxTextureImageUnits);
    m_context->getIntegerv(GraphicsContext3D::MAX_FRAGMENT_UNIFORM_VECTORS, &ANGLEResources.MaxFragmentUniformVectors);

    ANGLEResources.MaxDrawBuffers = 1;

    Extensions3D* extensions = m_context->getExtensions();
    if (extensions->supports("GL_ARB_texture_rectangle"))
        ANGLEResources.ARB_texture_rectangle = 1;

    m_context->m_compiler.setResources(ANGLEResources);
}
