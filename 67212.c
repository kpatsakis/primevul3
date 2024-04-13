static int parse_segments(struct MACH0_(obj_t)* bin, ut64 off) {
	int i, j, k, sect, len;
	ut32 size_sects;
	ut8 segcom[sizeof (struct MACH0_(segment_command))] = {0};
	ut8 sec[sizeof (struct MACH0_(section))] = {0};

	if (!UT32_MUL (&size_sects, bin->nsegs, sizeof (struct MACH0_(segment_command)))) {
		return false;
	}
	if (!size_sects || size_sects > bin->size) {
		return false;
	}
	if (off > bin->size || off + sizeof (struct MACH0_(segment_command)) > bin->size) {
		return false;
	}
	if (!(bin->segs = realloc (bin->segs, bin->nsegs * sizeof(struct MACH0_(segment_command))))) {
		perror ("realloc (seg)");
		return false;
	}
	j = bin->nsegs - 1;
	len = r_buf_read_at (bin->b, off, segcom, sizeof (struct MACH0_(segment_command)));
	if (len != sizeof (struct MACH0_(segment_command))) {
		bprintf ("Error: read (seg)\n");
		return false;
	}
	i = 0;
	bin->segs[j].cmd = r_read_ble32 (&segcom[i], bin->big_endian);
	i += sizeof (ut32);
	bin->segs[j].cmdsize = r_read_ble32 (&segcom[i], bin->big_endian);
	i += sizeof (ut32);
	memcpy (&bin->segs[j].segname, &segcom[i], 16);
	i += 16;
#if R_BIN_MACH064
	bin->segs[j].vmaddr = r_read_ble64 (&segcom[i], bin->big_endian);
	i += sizeof (ut64);
	bin->segs[j].vmsize = r_read_ble64 (&segcom[i], bin->big_endian);
	i += sizeof (ut64);
	bin->segs[j].fileoff = r_read_ble64 (&segcom[i], bin->big_endian);
	i += sizeof (ut64);
	bin->segs[j].filesize = r_read_ble64 (&segcom[i], bin->big_endian);
	i += sizeof (ut64);
#else
	bin->segs[j].vmaddr = r_read_ble32 (&segcom[i], bin->big_endian);
	i += sizeof (ut32);
	bin->segs[j].vmsize = r_read_ble32 (&segcom[i], bin->big_endian);
	i += sizeof (ut32);
	bin->segs[j].fileoff = r_read_ble32 (&segcom[i], bin->big_endian);
	i += sizeof (ut32);
	bin->segs[j].filesize = r_read_ble32 (&segcom[i], bin->big_endian);
	i += sizeof (ut32);
#endif
	bin->segs[j].maxprot = r_read_ble32 (&segcom[i], bin->big_endian);
	i += sizeof (ut32);
	bin->segs[j].initprot = r_read_ble32 (&segcom[i], bin->big_endian);
	i += sizeof (ut32);
	bin->segs[j].nsects = r_read_ble32 (&segcom[i], bin->big_endian);
	i += sizeof (ut32);
	bin->segs[j].flags = r_read_ble32 (&segcom[i], bin->big_endian);

	sdb_num_set (bin->kv, sdb_fmt ("mach0_segment_%d.offset", j), off, 0);
	sdb_num_set (bin->kv, "mach0_segments.count", 0, 0);
	sdb_set (bin->kv, "mach0_segment.format",
		"xd[16]zxxxxoodx "
		"cmd cmdsize segname vmaddr vmsize "
		"fileoff filesize maxprot initprot nsects flags", 0);

	if (bin->segs[j].nsects > 0) {
		sect = bin->nsects;
		bin->nsects += bin->segs[j].nsects;
		if (bin->nsects > 128) {
			int new_nsects = bin->nsects & 0xf;
			bprintf ("WARNING: mach0 header contains too many sections (%d). Wrapping to %d\n",
				 bin->nsects, new_nsects);
			bin->nsects = new_nsects;
		}
		if ((int)bin->nsects < 1) {
			bprintf ("Warning: Invalid number of sections\n");
			bin->nsects = sect;
			return false;
		}
		if (!UT32_MUL (&size_sects, bin->nsects-sect, sizeof (struct MACH0_(section)))){
			bin->nsects = sect;
			return false;
		}
		if (!size_sects || size_sects > bin->size){
			bin->nsects = sect;
			return false;
		}

		if (bin->segs[j].cmdsize != sizeof (struct MACH0_(segment_command)) \
				  + (sizeof (struct MACH0_(section))*bin->segs[j].nsects)){
			bin->nsects = sect;
			return false;
		}

		if (off + sizeof (struct MACH0_(segment_command)) > bin->size ||\
				off + sizeof (struct MACH0_(segment_command)) + size_sects > bin->size){
			bin->nsects = sect;
			return false;
		}

		if (!(bin->sects = realloc (bin->sects, bin->nsects * sizeof (struct MACH0_(section))))) {
			perror ("realloc (sects)");
			bin->nsects = sect;
			return false;
		}

		for (k = sect, j = 0; k < bin->nsects; k++, j++) {
			ut64 offset = off + sizeof (struct MACH0_(segment_command)) + j * sizeof (struct MACH0_(section));
			len = r_buf_read_at (bin->b, offset, sec, sizeof (struct MACH0_(section)));
			if (len != sizeof (struct MACH0_(section))) {
				bprintf ("Error: read (sects)\n");
				bin->nsects = sect;
				return false;
			}

			i = 0;
			memcpy (&bin->sects[k].sectname, &sec[i], 16);
			i += 16;
			memcpy (&bin->sects[k].segname, &sec[i], 16);
			bin->sects[k].segname[15] = 0;
			i += 16;
#if R_BIN_MACH064
			bin->sects[k].addr = r_read_ble64 (&sec[i], bin->big_endian);
			i += sizeof (ut64);
			bin->sects[k].size = r_read_ble64 (&sec[i], bin->big_endian);
			i += sizeof (ut64);
#else
			bin->sects[k].addr = r_read_ble32 (&sec[i], bin->big_endian);
			i += sizeof (ut32);
			bin->sects[k].size = r_read_ble32 (&sec[i], bin->big_endian);
			i += sizeof (ut32);
#endif
			bin->sects[k].offset = r_read_ble32 (&sec[i], bin->big_endian);
			i += sizeof (ut32);
			bin->sects[k].align = r_read_ble32 (&sec[i], bin->big_endian);
			i += sizeof (ut32);
			bin->sects[k].reloff = r_read_ble32 (&sec[i], bin->big_endian);
			i += sizeof (ut32);
			bin->sects[k].nreloc = r_read_ble32 (&sec[i], bin->big_endian);
			i += sizeof (ut32);
			bin->sects[k].flags = r_read_ble32 (&sec[i], bin->big_endian);
			i += sizeof (ut32);
			bin->sects[k].reserved1 = r_read_ble32 (&sec[i], bin->big_endian);
			i += sizeof (ut32);
			bin->sects[k].reserved2 = r_read_ble32 (&sec[i], bin->big_endian);
#if R_BIN_MACH064
			i += sizeof (ut32);
			bin->sects[k].reserved3 = r_read_ble32 (&sec[i], bin->big_endian);
#endif
		}
	}
	return true;
}
