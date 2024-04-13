std::unique_ptr<GroupFromFirstDocumentTransformation> GroupFromFirstDocumentTransformation::create(
    const intrusive_ptr<ExpressionContext>& expCtx,
    const std::string& groupId,
    vector<pair<std::string, intrusive_ptr<Expression>>> accumulatorExprs) {
    return std::make_unique<GroupFromFirstDocumentTransformation>(groupId,
                                                                  std::move(accumulatorExprs));
}