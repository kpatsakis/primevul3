static void collapse_path(char *filename)
{
	unsigned int currpath_len;
	char *currpath;
	char *r, *w;


	if (filename[0] == '/') {
		++filename;
	}


	currpath = filename;
	w = filename;

	for (r = filename; *r != '\0'; ++r) {
		*w++ = *r;


		if (*r == '/') {

			currpath_len = w - currpath - 1;


			if (currpath_len == 0
			 || (currpath_len == 1 && currpath[0] == '.')) {
				w = currpath;


			} else if (currpath_len == 2
			        && currpath[0] == '.' && currpath[1] == '.') {


				if (currpath == filename) {
					w = filename;
				} else {
					w = currpath - 1;

					while (w > filename) {
						if (*(w - 1) == '/') {
							break;
						}
						--w;
					}

					currpath = w;
				}


			} else {
				currpath = w;
			}
		}
	}

	*w = '\0';
}
