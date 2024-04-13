void ElementData::deref()
{
    if (!derefBase())
        return;

    if (m_isUnique)
        delete static_cast<UniqueElementData*>(this);
    else
        delete static_cast<ShareableElementData*>(this);
}
