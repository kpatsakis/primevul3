    void set_less(handle h, handle lh) { m_nodes[h].lt &= 0x80000000; m_nodes[h].lt |= lh; }
