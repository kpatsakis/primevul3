RenderMenuList::~RenderMenuList()
{
    if (m_popup)
        m_popup->disconnectClient();
    m_popup = 0;
}
