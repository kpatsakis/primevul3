void DocumentSourceGroup::doDispose() {
    // Free our resources.
    _groups = pExpCtx->getValueComparator().makeUnorderedValueMap<Accumulators>();
    _sorterIterator.reset();

    // Make us look done.
    groupsIterator = _groups->end();
}