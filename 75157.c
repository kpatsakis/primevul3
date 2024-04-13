unsigned Cache::liveCapacity() const 
{ 
    return m_capacity - deadCapacity();
}
