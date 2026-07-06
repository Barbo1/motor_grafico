#include <cstdint>

class Arena {
  private:
    std::size_t size;
    void* init_ptr, *curr_ptr;
  public:

    Arena(std::size_t size);

    void* alloc(std::size_t size, std::size_t alignment);
    void restart();
};

/*
class GrowableArena {
  private:
    std::size_t size;
    void* init_ptr, *curr_ptr;
  public:

    GrowableArena(std::size_t size);

    void* alloc(std::size_t size, std::size_t alignment);
    void restart();
};
*/
