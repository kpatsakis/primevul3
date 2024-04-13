void visitDOMNodes(DOMWrapperMap<Node>::Visitor* visitor)
{
    v8::HandleScope scope;

    DOMDataList& list = DOMDataStore::allStores();
    for (size_t i = 0; i < list.size(); ++i) {
        DOMDataStore* store = list[i];

        store->domNodeMap().visit(store, visitor);
    }
}
