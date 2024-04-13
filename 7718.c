bool DocumentSourceGroup::pathIncludedInGroupKeys(const std::string& dottedPath) const {
    return std::any_of(
        _idExpressions.begin(), _idExpressions.end(), [&dottedPath](const auto& exp) {
            if (auto fieldExp = dynamic_cast<ExpressionFieldPath*>(exp.get())) {
                if (fieldExp->representsPath(dottedPath)) {
                    return true;
                }
            }
            return false;
        });
}