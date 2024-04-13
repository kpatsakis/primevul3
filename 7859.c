    ~TensorInfoCache() {
      tensorflow::mutex_lock ml(lock);
      for (auto& p : entries) {
        libxsmm_spmdm_destroy(&p.second->handle);
      }
      entries.clear();
    }