Value DocumentSourceGroup::serialize(boost::optional<ExplainOptions::Verbosity> explain) const {
    MutableDocument insides;

    // Add the _id.
    if (_idFieldNames.empty()) {
        invariant(_idExpressions.size() == 1);
        insides["_id"] = _idExpressions[0]->serialize(static_cast<bool>(explain));
    } else {
        // Decomposed document case.
        invariant(_idExpressions.size() == _idFieldNames.size());
        MutableDocument md;
        for (size_t i = 0; i < _idExpressions.size(); i++) {
            md[_idFieldNames[i]] = _idExpressions[i]->serialize(static_cast<bool>(explain));
        }
        insides["_id"] = md.freezeToValue();
    }

    // Add the remaining fields.
    for (auto&& accumulatedField : _accumulatedFields) {
        intrusive_ptr<AccumulatorState> accum = accumulatedField.makeAccumulator();
        insides[accumulatedField.fieldName] =
            Value(accum->serialize(accumulatedField.expr.initializer,
                                   accumulatedField.expr.argument,
                                   static_cast<bool>(explain)));
    }

    if (_doingMerge) {
        // This makes the output unparsable (with error) on pre 2.6 shards, but it will never
        // be sent to old shards when this flag is true since they can't do a merge anyway.
        insides["$doingMerge"] = Value(true);
    }

    return Value(DOC(getSourceName() << insides.freeze()));
}