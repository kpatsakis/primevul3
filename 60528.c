evutil_read_file_(const char *filename, char **content_out, size_t *len_out,
    int is_binary)
{
	int fd, r;
	struct stat st;
	char *mem;
	size_t read_so_far=0;
	int mode = O_RDONLY;

	EVUTIL_ASSERT(content_out);
	EVUTIL_ASSERT(len_out);
	*content_out = NULL;
	*len_out = 0;

#ifdef O_BINARY
	if (is_binary)
		mode |= O_BINARY;
#endif

	fd = evutil_open_closeonexec_(filename, mode, 0);
	if (fd < 0)
		return -1;
	if (fstat(fd, &st) || st.st_size < 0 ||
	    st.st_size > EV_SSIZE_MAX-1 ) {
		close(fd);
		return -2;
	}
	mem = mm_malloc((size_t)st.st_size + 1);
	if (!mem) {
		close(fd);
		return -2;
	}
	read_so_far = 0;
#ifdef _WIN32
#define N_TO_READ(x) ((x) > INT_MAX) ? INT_MAX : ((int)(x))
#else
#define N_TO_READ(x) (x)
#endif
	while ((r = read(fd, mem+read_so_far, N_TO_READ(st.st_size - read_so_far))) > 0) {
		read_so_far += r;
		if (read_so_far >= (size_t)st.st_size)
			break;
		EVUTIL_ASSERT(read_so_far < (size_t)st.st_size);
	}
	close(fd);
	if (r < 0) {
		mm_free(mem);
		return -2;
	}
	mem[read_so_far] = 0;

	*len_out = read_so_far;
	*content_out = mem;
	return 0;
}
