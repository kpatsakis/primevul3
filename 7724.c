DepsTracker::State DocumentSourceGroup::getDependencies(DepsTracker* deps) const {
    // add the _id
    for (size_t i = 0; i < _idExpressions.size(); i++) {
        _idExpressions[i]->addDependencies(deps);
    }

    // add the rest
    for (auto&& accumulatedField : _accumulatedFields) {
        accumulatedField.expr.argument->addDependencies(deps);
        // Don't add initializer, because it doesn't refer to docs from the input stream.
    }

    return DepsTracker::State::EXHAUSTIVE_ALL;
}