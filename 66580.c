static int java_new_method (ut64 method_start) {
	METHOD_START = method_start;
	r_java_new_method ();
	return 0;
}
