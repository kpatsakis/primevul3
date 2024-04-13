DocumentSource::GetNextResult DocumentSourceGroup::doGetNext() {
    if (!_initialized) {
        const auto initializationResult = initialize();
        if (initializationResult.isPaused()) {
            return initializationResult;
        }
        invariant(initializationResult.isEOF());
    }

    for (auto&& accum : _currentAccumulators) {
        accum->reset();  // Prep accumulators for a new group.
    }

    if (_spilled) {
        return getNextSpilled();
    } else {
        return getNextStandard();
    }
}