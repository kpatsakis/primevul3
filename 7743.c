Document GroupFromFirstDocumentTransformation::applyTransformation(const Document& input) {
    MutableDocument output(_accumulatorExprs.size());

    for (auto&& expr : _accumulatorExprs) {
        auto value = expr.second->evaluate(input, &expr.second->getExpressionContext()->variables);
        output.addField(expr.first, value.missing() ? Value(BSONNULL) : value);
    }

    return output.freeze();
}