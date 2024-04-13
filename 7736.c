DocumentSourceGroup::DocumentSourceGroup(const intrusive_ptr<ExpressionContext>& pExpCtx,
                                         boost::optional<size_t> maxMemoryUsageBytes)
    : DocumentSource(kStageName, pExpCtx),
      _usedDisk(false),
      _doingMerge(false),
      _memoryTracker{pExpCtx->allowDiskUse && !pExpCtx->inMongos,
                     maxMemoryUsageBytes ? *maxMemoryUsageBytes
                                         : internalDocumentSourceGroupMaxMemoryBytes.load()},
      // We spill to disk in debug mode, regardless of allowDiskUse, to stress the system.
      _file(!pExpCtx->inMongos && (pExpCtx->allowDiskUse || kDebugBuild)
                ? std::make_shared<Sorter<Value, Value>::File>(pExpCtx->tempDir + "/" +
                                                               nextFileName())
                : nullptr),
      _initialized(false),
      _groups(pExpCtx->getValueComparator().makeUnorderedValueMap<Accumulators>()),
      _spilled(false) {}