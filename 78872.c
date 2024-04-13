static Attr* findAttrNodeInList(AttrNodeList* attrNodeList, const QualifiedName& name)
{
    for (unsigned i = 0; i < attrNodeList->size(); ++i) {
        if (attrNodeList->at(i)->qualifiedName() == name)
            return attrNodeList->at(i).get();
    }
    return 0;
}
