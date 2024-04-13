static int check_array_args(unsigned int cmd, void *parg, size_t *array_size,
			    void * __user *user_ptr, void ***kernel_ptr)
{
	int ret = 0;

	switch (cmd) {
	case VIDIOC_QUERYBUF:
	case VIDIOC_QBUF:
	case VIDIOC_DQBUF: {
		struct v4l2_buffer *buf = parg;

		if (V4L2_TYPE_IS_MULTIPLANAR(buf->type) && buf->length > 0) {
			if (buf->length > VIDEO_MAX_PLANES) {
				ret = -EINVAL;
				break;
			}
			*user_ptr = (void __user *)buf->m.planes;
			*kernel_ptr = (void **)&buf->m.planes;
			*array_size = sizeof(struct v4l2_plane) * buf->length;
			ret = 1;
		}
		break;
	}

	case VIDIOC_S_EXT_CTRLS:
	case VIDIOC_G_EXT_CTRLS:
	case VIDIOC_TRY_EXT_CTRLS: {
		struct v4l2_ext_controls *ctrls = parg;

		if (ctrls->count != 0) {
			*user_ptr = (void __user *)ctrls->controls;
			*kernel_ptr = (void **)&ctrls->controls;
			*array_size = sizeof(struct v4l2_ext_control)
				    * ctrls->count;
			ret = 1;
		}
		break;
	}
	}

 	return ret;
 }
