DepsTracker::State GroupFromFirstDocumentTransformation::addDependencies(DepsTracker* deps) const {
    for (auto&& expr : _accumulatorExprs) {
        expr.second->addDependencies(deps);
    }

    // This stage will replace the entire document with a new document, so any existing fields
    // will be replaced and cannot be required as dependencies. We use EXHAUSTIVE_ALL here
    // instead of EXHAUSTIVE_FIELDS, as in ReplaceRootTransformation, because the stages that
    // follow a $group stage should not depend on document metadata.
    return DepsTracker::State::EXHAUSTIVE_ALL;
}