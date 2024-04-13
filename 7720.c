bool DocumentSourceGroup::MemoryUsageTracker::shouldSpillWithAttemptToSaveMemory(
    std::function<int()> saveMemory) {
    if (!allowDiskUse && (memoryUsageBytes > maxMemoryUsageBytes)) {
        memoryUsageBytes -= saveMemory();
    }

    if (memoryUsageBytes > maxMemoryUsageBytes) {
        uassert(ErrorCodes::QueryExceededMemoryLimitNoDiskUseAllowed,
                "Exceeded memory limit for $group, but didn't allow external sort."
                " Pass allowDiskUse:true to opt in.",
                allowDiskUse);
        memoryUsageBytes = 0;
        return true;
    }
    return false;
}