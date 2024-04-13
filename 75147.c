Cache::Cache()
: m_disabled(false)
, m_pruneEnabled(true)
, m_capacity(cDefaultCacheCapacity)
, m_minDeadCapacity(0)
, m_maxDeadCapacity(cDefaultCacheCapacity)
, m_liveSize(0)
, m_deadSize(0)
{
}
