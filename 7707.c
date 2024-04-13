intrusive_ptr<DocumentSourceGroup> DocumentSourceGroup::create(
    const intrusive_ptr<ExpressionContext>& pExpCtx,
    const boost::intrusive_ptr<Expression>& groupByExpression,
    std::vector<AccumulationStatement> accumulationStatements,
    boost::optional<size_t> maxMemoryUsageBytes) {
    size_t memoryBytes = maxMemoryUsageBytes ? *maxMemoryUsageBytes
                                             : internalDocumentSourceGroupMaxMemoryBytes.load();
    intrusive_ptr<DocumentSourceGroup> groupStage(new DocumentSourceGroup(pExpCtx, memoryBytes));
    groupStage->setIdExpression(groupByExpression);
    for (auto&& statement : accumulationStatements) {
        groupStage->addAccumulator(statement);
    }

    return groupStage;
}