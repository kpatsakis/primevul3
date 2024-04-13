void DocumentSourceGroup::addAccumulator(AccumulationStatement accumulationStatement) {
    _accumulatedFields.push_back(accumulationStatement);
}