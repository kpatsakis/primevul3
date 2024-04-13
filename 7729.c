intrusive_ptr<DocumentSource> DocumentSourceGroup::createFromBson(
    BSONElement elem, const intrusive_ptr<ExpressionContext>& pExpCtx) {
    uassert(15947, "a group's fields must be specified in an object", elem.type() == Object);

    intrusive_ptr<DocumentSourceGroup> pGroup(new DocumentSourceGroup(pExpCtx));

    BSONObj groupObj(elem.Obj());
    BSONObjIterator groupIterator(groupObj);
    VariablesParseState vps = pExpCtx->variablesParseState;
    while (groupIterator.more()) {
        BSONElement groupField(groupIterator.next());
        StringData pFieldName = groupField.fieldNameStringData();
        if (pFieldName == "_id") {
            uassert(
                15948, "a group's _id may only be specified once", pGroup->_idExpressions.empty());
            pGroup->setIdExpression(parseIdExpression(pExpCtx, groupField, vps));
            invariant(!pGroup->_idExpressions.empty());
        } else if (pFieldName == "$doingMerge") {
            massert(17030, "$doingMerge should be true if present", groupField.Bool());

            pGroup->setDoingMerge(true);
        } else {
            // Any other field will be treated as an accumulator specification.
            pGroup->addAccumulator(
                AccumulationStatement::parseAccumulationStatement(pExpCtx, groupField, vps));
        }
    }

    uassert(15955, "a group specification must include an _id", !pGroup->_idExpressions.empty());
    return pGroup;
}