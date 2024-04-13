DocumentSource::GetModPathsReturn GroupFromFirstDocumentTransformation::getModifiedPaths() const {
    // Replaces the entire root, so all paths are modified.
    return {DocumentSource::GetModPathsReturn::Type::kAllPaths, std::set<std::string>{}, {}};
}