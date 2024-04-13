StringMap<boost::intrusive_ptr<Expression>> DocumentSourceGroup::getIdFields() const {
    if (_idFieldNames.empty()) {
        invariant(_idExpressions.size() == 1);
        return {{"_id", _idExpressions[0]}};
    } else {
        invariant(_idFieldNames.size() == _idExpressions.size());
        StringMap<boost::intrusive_ptr<Expression>> result;
        for (std::size_t i = 0; i < _idFieldNames.size(); ++i) {
            result["_id." + _idFieldNames[i]] = _idExpressions[i];
        }
        return result;
    }
}