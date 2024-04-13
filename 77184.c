void visitActiveDOMObjects(DOMWrapperMap<void>::Visitor* visitor)
{
    v8::HandleScope scope;

    DOMDataList& list = DOMDataStore::allStores();
    for (size_t i = 0; i < list.size(); ++i) {
        DOMDataStore* store = list[i];

        store->activeDomObjectMap().visit(store, visitor);
    }
}
