 static void dbgbuf(unsigned int cmd, struct video_device *vfd,
 					struct v4l2_buffer *p)
 {
	struct v4l2_timecode *tc = &p->timecode;
	struct v4l2_plane *plane;
	int i;

	dbgarg(cmd, "%02ld:%02d:%02d.%08ld index=%d, type=%s, "
		"flags=0x%08d, field=%0d, sequence=%d, memory=%s\n",
			p->timestamp.tv_sec / 3600,
			(int)(p->timestamp.tv_sec / 60) % 60,
			(int)(p->timestamp.tv_sec % 60),
			(long)p->timestamp.tv_usec,
			p->index,
			prt_names(p->type, v4l2_type_names),
			p->flags, p->field, p->sequence,
			prt_names(p->memory, v4l2_memory_names));

	if (V4L2_TYPE_IS_MULTIPLANAR(p->type) && p->m.planes) {
		for (i = 0; i < p->length; ++i) {
			plane = &p->m.planes[i];
			dbgarg2("plane %d: bytesused=%d, data_offset=0x%08x "
				"offset/userptr=0x%08lx, length=%d\n",
				i, plane->bytesused, plane->data_offset,
				plane->m.userptr, plane->length);
		}
	} else {
		dbgarg2("bytesused=%d, offset/userptr=0x%08lx, length=%d\n",
			p->bytesused, p->m.userptr, p->length);
	}

	dbgarg2("timecode=%02d:%02d:%02d type=%d, "
		"flags=0x%08d, frames=%d, userbits=0x%08x\n",
			tc->hours, tc->minutes, tc->seconds,
			tc->type, tc->flags, tc->frames, *(__u32 *)tc->userbits);
}
