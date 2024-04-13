template<class T, size_t n> static void removeFromVector(Vector<T*, n> & vec, T* item)
{
    size_t size = vec.size();
    for (size_t i = 0; i != size; ++i)
        if (vec[i] == item) {
            vec.remove(i);
            break;
        }
}
