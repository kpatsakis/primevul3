Document GroupFromFirstDocumentTransformation::serializeTransformation(
    boost::optional<ExplainOptions::Verbosity> explain) const {

    MutableDocument newRoot(_accumulatorExprs.size());
    for (auto&& expr : _accumulatorExprs) {
        newRoot.addField(expr.first, expr.second->serialize(static_cast<bool>(explain)));
    }

    return {{"newRoot", newRoot.freezeToValue()}};
}