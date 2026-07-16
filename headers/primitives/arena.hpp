#pragma once

#include <cstddef>

struct ArenaConstexFlag {
  std::byte* context_current;
  std::size_t context_remaining;
};

class Arena {
  private:
    std::byte* init_ptr;
    std::byte* curr_ptr;
    std::size_t size, remaining;
  public:
    Arena(std::size_t size);
    ~Arena();

    std::byte* allocd(std::size_t size, std::size_t alignment = 1);
    std::byte* allocp(std::size_t size, std::size_t pot = 0);

    template<typename T> T* talloc(std::size_t size) {
      return (T*)(this->allocp(size * sizeof(T)));
    }

    template<typename T> T* atalloc(std::size_t size) {
      return (T*)(this->allocd(size * sizeof(T), alignof(T)));
    }

    ArenaConstexFlag get_context();
    void go_back_context(ArenaConstexFlag);

    void restart();
};
