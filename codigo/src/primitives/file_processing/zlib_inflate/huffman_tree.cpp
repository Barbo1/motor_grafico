#pragma once

#include <cstdint>
#include <array>
#include <algorithm>

struct huffnode {
  int64_t elem;
  huffnode* branch[2];
};

/* Huffman trees that follows the RFC-1951 specification. It use statical ONLY memory. 
 * */
class HuffmanTree {
  private: 
    huffnode* root;

    static const std::size_t BL_SIZE = 300;
    static const std::size_t NEXT_SIZE = 300;
    static std::array<uint64_t, 300> bl_count;
    static std::array<uint64_t, 300> next_code;
  
  public:
    HuffmanTree (const std::array<uint8_t, 317>& lenghts, uint32_t size, bool* error);
    ~HuffmanTree ();

    class iterator {
      private:
        uint8_t height;
        uint64_t bits;
        std::array<huffnode*, 64> pointed;
        huffnode* root;

      public:
        iterator (const HuffmanTree& tree);
        bool advance (bool bit);
        void go_back ();
        bool finished ();
        uint8_t hieght();

        uint64_t operator* ();
        uint16_t operator& ();
    };
};

std::array<uint64_t, HuffmanTree::BL_SIZE> HuffmanTree::bl_count {};
std::array<uint64_t, HuffmanTree::NEXT_SIZE> HuffmanTree::next_code {};

HuffmanTree::HuffmanTree (const std::array<uint8_t, 317>& lengths, uint32_t size, bool* error) {
  *error = false;
  uint8_t greater = *std::max_element (lengths.begin(), lengths.begin() + size);
  std::fill_n (this->bl_count.begin(), greater+1, 0);
  for (uint32_t i = 0; i < size; i++)
    this->bl_count[lengths[i]]++;

  this->bl_count[0] = 0;
  for (int bits = 1; bits <= greater; bits++)
    this->next_code[bits] = (this->next_code[bits-1] + this->bl_count[bits-1]) << 1;

  huffnode* node;
  this->root = new huffnode { .elem = -1, .branch = {nullptr, nullptr} };

  int len;
  for (uint32_t n = 0; n < size; n++) {
    len = lengths[n];
    if (len != 0) {
      node = this->root;
      uint64_t aux = this->next_code[len]++;
      for (int64_t j = len - 1; j > -1; j--) {
        uint64_t cond = (aux >> j) & 1;
        if (node->branch[cond] == nullptr)
          node->branch[cond] = new huffnode { .elem = -1, .branch = {nullptr, nullptr} };
        else if (node->branch[cond]->elem != -1) {
          *error = true;
          return;
        }
        node = node->branch[cond];
      }
      node->elem = n;
    }
  }
}

static void delete_nodes (huffnode* node) {
  if (node != nullptr) {
    delete_nodes(node->branch[0]);
    delete_nodes(node->branch[1]);
    delete node;
  }
}

HuffmanTree::~HuffmanTree () {
  if (this->root != nullptr) {
    delete_nodes(this->root);
  }
}

HuffmanTree::iterator::iterator (const HuffmanTree& tree) {
  this->pointed[0] = this->root = tree.root;
  this->height = 0;
  this->bits = 0;
}

bool HuffmanTree::iterator::advance (bool bit) {
  huffnode * pointed = this->pointed[this->height];
  if (bit && pointed->branch[1] != nullptr) {
    this->bits |= 1 << this->height++;
    this->pointed[this->height] = pointed->branch[1];
    return true;
  } else if (!bit && pointed->branch[1] != nullptr) {
    this->pointed[++this->height] = pointed->branch[0];
    return true;
  } else return false;
}

void HuffmanTree::iterator::go_back () {
  this->bits = 0;
  this->height = 0;
}

bool HuffmanTree::iterator::finished () {
  return this->pointed[this->height]->elem != -1;
}

uint8_t HuffmanTree::iterator::hieght () {
  return this->height;
}

uint64_t HuffmanTree::iterator::operator* () {
  return this->bits;
}

uint16_t HuffmanTree::iterator::operator& () {
  return this->pointed[this->height]->elem;
}
