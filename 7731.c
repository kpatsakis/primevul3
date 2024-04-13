    int operator()(const Data& lhs, const Data& rhs) const {
        return _valueComparator.compare(lhs.first, rhs.first);
    }