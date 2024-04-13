bool DocumentSourceGroup::canRunInParallelBeforeWriteStage(
    const std::set<std::string>& nameOfShardKeyFieldsUponEntryToStage) const {
    if (_doingMerge) {
        return true;  // This is fine.
    }

    // Certain $group stages are allowed to execute on each exchange consumer. In order to
    // guarantee each consumer will only group together data from its own shard, the $group must
    // group on a superset of the shard key.
    for (auto&& currentPathOfShardKey : nameOfShardKeyFieldsUponEntryToStage) {
        if (!pathIncludedInGroupKeys(currentPathOfShardKey)) {
            // This requires an exact path match, but as a future optimization certain path
            // prefixes should be okay. For example, if the shard key path is "a.b", and we're
            // grouping by "a", then each group of "a" is strictly more specific than "a.b", so
            // we can deduce that grouping by "a" will not need to group together documents
            // across different values of the shard key field "a.b", and thus as long as any
            // other shard key fields are similarly preserved will not need to consume a merged
            // stream to perform the group.
            return false;
        }
    }
    return true;
}