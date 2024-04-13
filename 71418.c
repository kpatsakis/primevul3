static void __show_smap(struct seq_file *m, const struct mem_size_stats *mss)
{
	SEQ_PUT_DEC("Rss:            ", mss->resident);
	SEQ_PUT_DEC(" kB\nPss:            ", mss->pss >> PSS_SHIFT);
	SEQ_PUT_DEC(" kB\nShared_Clean:   ", mss->shared_clean);
	SEQ_PUT_DEC(" kB\nShared_Dirty:   ", mss->shared_dirty);
	SEQ_PUT_DEC(" kB\nPrivate_Clean:  ", mss->private_clean);
	SEQ_PUT_DEC(" kB\nPrivate_Dirty:  ", mss->private_dirty);
	SEQ_PUT_DEC(" kB\nReferenced:     ", mss->referenced);
	SEQ_PUT_DEC(" kB\nAnonymous:      ", mss->anonymous);
	SEQ_PUT_DEC(" kB\nLazyFree:       ", mss->lazyfree);
	SEQ_PUT_DEC(" kB\nAnonHugePages:  ", mss->anonymous_thp);
	SEQ_PUT_DEC(" kB\nShmemPmdMapped: ", mss->shmem_thp);
	SEQ_PUT_DEC(" kB\nShared_Hugetlb: ", mss->shared_hugetlb);
	seq_put_decimal_ull_width(m, " kB\nPrivate_Hugetlb: ",
				  mss->private_hugetlb >> 10, 7);
	SEQ_PUT_DEC(" kB\nSwap:           ", mss->swap);
	SEQ_PUT_DEC(" kB\nSwapPss:        ",
					mss->swap_pss >> PSS_SHIFT);
	SEQ_PUT_DEC(" kB\nLocked:         ",
					mss->pss_locked >> PSS_SHIFT);
	seq_puts(m, " kB\n");
}
