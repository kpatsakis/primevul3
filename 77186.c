DOMDataStoreHandle::~DOMDataStoreHandle()
{
    if (m_store)
        V8PerIsolateData::current()->unregisterDOMDataStore(m_store.get());
}
