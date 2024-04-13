String Element::textFromChildren()
{
    Text* firstTextNode = 0;
    bool foundMultipleTextNodes = false;
    unsigned totalLength = 0;

    for (Node* child = firstChild(); child; child = child->nextSibling()) {
        if (!child->isTextNode())
            continue;
        Text* text = toText(child);
        if (!firstTextNode)
            firstTextNode = text;
        else
            foundMultipleTextNodes = true;
        unsigned length = text->data().length();
        if (length > std::numeric_limits<unsigned>::max() - totalLength)
            return emptyString();
        totalLength += length;
    }

    if (!firstTextNode)
        return emptyString();

    if (firstTextNode && !foundMultipleTextNodes) {
        firstTextNode->atomize();
        return firstTextNode->data();
    }

    StringBuilder content;
    content.reserveCapacity(totalLength);
    for (Node* child = firstTextNode; child; child = child->nextSibling()) {
        if (!child->isTextNode())
            continue;
        content.append(toText(child)->data());
    }

    ASSERT(content.length() == totalLength);
    return content.toString();
}
