ShapeRefiner::ShapeRefiner(const VersionDef& versions,
                           const OpRegistryInterface* ops)
    : ShapeRefiner(versions.producer(), ops) {}