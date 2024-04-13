AXObject* DateTimeChooserImpl::rootAXObject()
{
    return m_popup ? m_popup->rootAXObject() : 0;
}
