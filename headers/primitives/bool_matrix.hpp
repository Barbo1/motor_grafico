#include <cmath>
#include <cstdint>
#include <cstring>

class BoolMatrixU {
  private:
    uint64_t * _data_;
    uint64_t * _parity_;
    uint64_t _lenr_;
    uint64_t _lenc_;
    uint64_t _rows_;
    uint64_t _columns_;

  public:
    BoolMatrixU (unsigned rows, unsigned columns) noexcept ;
    ~BoolMatrixU ();

    uint64_t get_width();
    uint64_t get_height();
    uint64_t get_lenc();
    uint64_t get_lenr();

    void set (uint64_t row, uint64_t column);
    void unset (uint64_t row, uint64_t column);
    void change (uint64_t row, uint64_t column, uint64_t bit);
    uint64_t operator() (unsigned row, unsigned column);

    void fill_bounds ();
    uint64_t number_bits (uint64_t lenr, uint64_t lenc);
    uint64_t number_bits_quad (uint64_t lenr, uint64_t lenc);
};

class BoolMatrixS {
  private:
    uint64_t * _data_;
    uint64_t * _parity_;
    uint64_t _lenr_;
    uint64_t _lenc_;
    uint64_t _rows_;
    uint64_t _columns_;

  public:
    BoolMatrixS (unsigned rows, unsigned columns) noexcept ;
    ~BoolMatrixS ();

    uint64_t get_width();
    uint64_t get_height();
    uint64_t get_lenc();
    uint64_t get_lenr();

    void set (uint64_t row, uint64_t column);
    void unset (uint64_t row, uint64_t column);
    void change (uint64_t row, uint64_t column, uint64_t bit);
    bool operator() (unsigned row, unsigned column);

    void fill_bounds ();
    uint64_t number_bits (uint64_t lenr, uint64_t lenc);
    uint64_t number_bits_quad (uint64_t lenr, uint64_t lenc);
};
