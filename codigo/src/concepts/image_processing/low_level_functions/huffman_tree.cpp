#pragma once

#include <vector>
#include <cstdint>
#include <array>

struct huffnode {
  uint64_t elem;
  bool final;
  huffnode* izq; // 1.
  huffnode* der; // 0.
};

static constexpr std::array<uint16_t, 19> symbol {
  16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15
};

static constexpr std::array<uint16_t, 19> inv_symbol {
  3, 17, 15, 13, 11, 9, 7, 5, 4, 6, 8, 10, 12, 14, 16, 18, 0, 1, 2
};

class HuffmanTree {
  private: 
    huffnode* root;

    static std::array<uint8_t, 300> bl_count;
    static std::array<uint8_t, 300> next_code;
  
  public:
    HuffmanTree (const std::vector<uint8_t>& vec, int type, bool* error);
    ~HuffmanTree ();

    class iterator {
      private:
        uint8_t height;
        uint64_t bits;
        std::vector<huffnode*> pointed;
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

std::array<uint8_t, 300> HuffmanTree::bl_count {0};
std::array<uint8_t, 300> HuffmanTree::next_code {0};

HuffmanTree::HuffmanTree (const std::vector<uint8_t>& lengths, int type, bool* error) {
  *error = false;
  uint8_t greater = lengths[0];
  for (auto& v: lengths)
    if (v > greater) greater = v;
  for (int i = 0; i <= greater; i++) 
    this->bl_count[i] = 0;
  for (auto& v: lengths)
    this->bl_count[v]++;

  this->bl_count[0] = 0;
  for (int bits = 1; bits <= greater; bits++)
    this->next_code[bits] = (this->next_code[bits-1] + this->bl_count[bits-1]) << 1;

  huffnode* node;
  this->root = new huffnode {
    .elem = 0,
    .final = false,
    .izq = nullptr, 
    .der = nullptr 
  };

  int len;
  for (int n = 0; n < lengths.size (); n++) {
    if (type == 1) 
      len = lengths[inv_symbol[n]];
    else
      len = lengths[n];
    if (len != 0) {
      node = this->root;
      uint8_t aux = this->next_code[len]++;
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
  this->root = tree.root;
  this->pointed = std::vector<huffnode*>();
  this->pointed.reserve (15);
  this->pointed.push_back (tree.root);
  this->height = 0;
  this->bits = 0;
}

bool HuffmanTree::iterator::advance (bool bit) {
  huffnode * pointed = this->pointed.back ();
  if (bit && pointed->der != nullptr) {
    this->pointed.push_back(pointed->der);
    this->bits |= 1 << this->height;
    this->height++;
    return true;
  } else if (!bit && pointed->izq != nullptr) {
    this->pointed.push_back(pointed->izq);
    this->bits &= ~(1 << this->height);
    this->height++;
    return true;
  } else return false;
}

void HuffmanTree::iterator::go_back () {
  this->pointed.clear();
  this->pointed.push_back(this->root);
  this->bits = 0;
  this->height = 0;
}

bool HuffmanTree::iterator::finished () {
  return this->pointed.back()->final;
}

uint8_t HuffmanTree::iterator::hieght () {
  return this->height;
}

uint64_t HuffmanTree::iterator::operator* () {
  return this->bits & ((1 << this->height) - 1);
}

uint16_t HuffmanTree::iterator::operator& () {
  return this->pointed.back()->elem;
}
