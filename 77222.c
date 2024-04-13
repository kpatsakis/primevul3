PassRefPtr<GraphicsContext3D> GraphicsContext3D::create(GraphicsContext3D::Attributes attrs, HostWindow* hostWindow, GraphicsContext3D::RenderStyle renderStyle)
{
    if (renderStyle == RenderDirectlyToHostWindow)
        return 0;
    RefPtr<GraphicsContext3D> context = adoptRef(new GraphicsContext3D(attrs, hostWindow, renderStyle));
    return context->m_private ? context.release() : 0;
}
