DOMDataStoreHandle::DOMDataStoreHandle(bool initialize)
    : m_store(adoptPtr(!initialize ? 0 : new ScopedDOMDataStore()))
{
    if (m_store)
        V8PerIsolateData::current()->registerDOMDataStore(m_store.get());
}
