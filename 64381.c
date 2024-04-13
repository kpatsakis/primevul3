ofproto_set_threads(int n_handlers_, int n_revalidators_)
{
    int threads = MAX(count_cpu_cores(), 2);

    n_revalidators = MAX(n_revalidators_, 0);
    n_handlers = MAX(n_handlers_, 0);

    if (!n_revalidators) {
        n_revalidators = n_handlers
            ? MAX(threads - (int) n_handlers, 1)
            : threads / 4 + 1;
    }

    if (!n_handlers) {
        n_handlers = MAX(threads - (int) n_revalidators, 1);
    }
}
