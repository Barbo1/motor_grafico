#pragma once

#include <cstdint>
#include <array>
#include <algorithm>

struct huffnode {
  uint64_t elem;
  bool final;
  huffnode* izq; // 0.
  huffnode* der; // 1.
  // change to 'huffnode* branch[2]{nullptr};'
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
    HuffmanTree (const std::array<uint8_t, 317>& lenghts, int size, bool* error);
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

HuffmanTree::HuffmanTree (const std::array<uint8_t, 317>& lengths, int size, bool* error) {
  *error = false;
  uint8_t greater = *std::max_element (lengths.begin(), lengths.begin() + size);
  std::fill_n (this->bl_count.begin(), greater+1, 0);
  for (int i = 0; i < size; i++)
    this->bl_count[lengths[i]]++;

  this->bl_count[0] = 0;
  for (int bits = 1; bits <= greater; bits++)
    this->next_code[bits] = (this->next_code[bits-1] + this->bl_count[bits-1]) << 1;

  huffnode* node;
  this->root = new huffnode { .elem = 0, .final = false, .izq = nullptr, .der = nullptr };

  int len;
  for (int n = 0; n < size; n++) {
    len = lengths[n];
    if (len != 0) {
      node = this->root;
      uint64_t aux = this->next_code[len]++;
      for (int8_t j = len - 1; j > -1; j--) {
        if ((1 << j) & aux) {
          if (node->der == nullptr)
            node->der = new huffnode { .elem = 0, .final = false, .izq = nullptr, .der = nullptr };
          else if (node->der->final) {
            *error = true;
            return;
          }
          node = node->der;
        } else {
          if (node->izq == nullptr)
            node->izq = new huffnode { .elem = 0, .final = false, .izq = nullptr, .der = nullptr };
          else if (node->izq->final) {
            *error = true;
            return;
          }
          node = node->izq;
        }
      }
      node->final = true;
      node->elem = n;
    }
  }
}

static void delete_nodes (huffnode* node) {
  if (node != nullptr) {
    delete_nodes(node->izq);
    delete_nodes(node->der);
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
  if (bit && pointed->der != nullptr) {
    this->bits |= 1 << this->height++;
    this->pointed[this->height] = pointed->der;
    return true;
  } else if (!bit && pointed->izq != nullptr) {
    this->pointed[++this->height] = pointed->izq;
    return true;
  } else return false;
}

void HuffmanTree::iterator::go_back () {
  this->bits = 0;
  this->height = 0;
}

bool HuffmanTree::iterator::finished () {
  return this->pointed[this->height]->final;
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
