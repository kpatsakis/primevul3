int DocumentSourceGroup::freeMemory() {
    invariant(_groups);
    int totalMemorySaved = 0;
    for (auto&& group : *_groups) {
        for (auto&& groupObj : group.second) {
            auto prevMemUsage = groupObj->memUsageForSorter();
            groupObj->reduceMemoryConsumptionIfAble();

            // Update the memory usage for this group.
            totalMemorySaved += (prevMemUsage - groupObj->memUsageForSorter());
        }
    }
    return totalMemorySaved;
}