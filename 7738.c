DocumentSource::GetNextResult DocumentSourceGroup::initialize() {
    GetNextResult input = pSource->getNext();
    return initializeSelf(input);
}