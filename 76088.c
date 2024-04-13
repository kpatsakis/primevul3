JSArray::~JSArray()
{
    ASSERT(vptr() == JSGlobalData::jsArrayVPtr);
    checkConsistency(DestructorConsistencyCheck);

    delete m_storage->m_sparseValueMap;
    fastFree(m_storage->m_allocBase);
}
