void jas_deprecated(const char *s)
{
	static char message[] =
	"WARNING!!! WARNING!!! WARNING!!! WARNING!!! WARNING!!! WARNING!!!\n"
	"WARNING!!! WARNING!!! WARNING!!! WARNING!!! WARNING!!! WARNING!!!\n"
	"WARNING!!! WARNING!!! WARNING!!! WARNING!!! WARNING!!! WARNING!!!\n"
	"YOUR CODE IS RELYING ON DEPRECATED FUNCTIONALTIY IN THE JASPER LIBRARY.\n"
	"THIS FUNCTIONALITY WILL BE REMOVED IN THE NEAR FUTURE.\n"
	"PLEASE FIX THIS PROBLEM BEFORE YOUR CODE STOPS WORKING!\n"
	;
	jas_eprintf("%s", message);
	jas_eprintf("The specific problem is as follows:\n%s\n", s);
}
