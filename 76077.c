    void set_balance_factor(handle h, int bf)
    {
        if (bf == 0) {
            m_nodes[h].lt &= 0x7FFFFFFF;
            m_nodes[h].gt &= 0x7FFFFFFF;
        } else {
            m_nodes[h].lt |= 0x80000000;
            if (bf < 0)
                m_nodes[h].gt |= 0x80000000;
            else
                m_nodes[h].gt &= 0x7FFFFFFF;
        }
    }
