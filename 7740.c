void GroupFromFirstDocumentTransformation::optimize() {
    for (auto&& expr : _accumulatorExprs) {
        expr.second = expr.second->optimize();
    }
}