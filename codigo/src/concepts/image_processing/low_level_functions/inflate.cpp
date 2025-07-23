#pragma once

#include "./huffman_tree.cpp"
#include "./access_bit.cpp"

#include <iostream>
#include <cstdint>
#include <array>
#include <vector>
#include <algorithm>

/* Symbols for the alphabet of lenghts. */
static constexpr std::array<uint16_t, 19> symbol {
  16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15
};

/* Arrays of precalculated lenghts and extra bits needed to compute the compressed content. 
 * */
static std::array<uint16_t, 29> lenght_code_lenghts = {
  3,4,5,6,7,8,9,10,11,13,15,17,19,23,27,31,35,43,51,59,67,83,99,115,131,163,195,227,258
};
static std::array<uint16_t, 29> lenght_code_extras = {
  0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,0
};
static std::array<uint16_t, 30> distance_code_lenghts = {
  1,2,3,4,5,7,9,13,17,25,33,49,65,97,129,193,257,385,513,769,1025,1537,2049,3073,4097,6145,8193,12289,16385,24577
};
static std::array<uint16_t, 30> distance_code_extras = {
  0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13
};

/* Function that takes the length code, and use it to read and return the 
 * total lenght obtained. 
 * */
uint16_t use_extra_len (uint8_t* iter, uint16_t lit_val, uint64_t& i) {
  lit_val -= 257;

  if (lit_val > 28)
    std::cout << "Problem reading lit_val" << std::endl;
  else if (lit_val == 28)
    return 258;

  uint16_t count_extra = lenght_code_extras [lit_val];
  uint16_t extra = 0;
  if (count_extra > 0) 
    extra = access_bit (iter, i, count_extra);

  return extra + lenght_code_lenghts [lit_val];
}

/* Function that takes the distance code, and use it to read and return the 
 * total distance obtained. 
 * */
uint16_t use_extra_dist (uint8_t* iter, uint16_t distc, uint64_t& i) {
  if (distc > 29)
    std::cout << "Problem reading distc" << std::endl;

  uint16_t count_extra = distance_code_extras [distc];
  uint16_t extra = 0;
  if (count_extra != 0) 
    extra = access_bit (iter, i, count_extra);

  return extra + distance_code_lenghts [distc];
}

/* Inflate algorithm, following the specification of RFC-1951. */
bool inflate (std::vector<uint8_t>& datastream, std::vector<uint8_t>& output) {
  output.reserve (datastream.size ());

  /* CMF and FLG comparation. */
  uint16_t cmf_flg = datastream[0] << 8 | datastream[1];
  if ((datastream[0] & 0x0F) != 8 || (cmf_flg % 31) != 0) {
    std::cout << "zilb error: cmf or flg error found." << std::endl;
    return false;
  }

  uint64_t pos = 16 | (datastream[1] & 0x20);

  /* begining of the iterations. */
  bool bfinal;
  uint64_t dist, len;
  uint64_t lit_val, hlit, hdist, hclen, byt;
  std::array<uint8_t, 317> lenghts;
  std::array<uint8_t, 317> aux;

  do {
    bfinal = access_bit (&datastream[0], pos, 1);
    switch (access_bit (&datastream[0], pos, 2)) {

      /* non-compressed block. */
      case 0:
        pos += ((pos & 7) > 0) * (8 - (pos & 7));
        len = pos >> 3;
        lit_val = datastream [len] | datastream [len + 1] << 8;
        len += 4;
        output.insert (
          output.end (), 
          datastream.begin () + len, 
          datastream.begin () + (len + lit_val)
        );
        pos += lit_val * 8 + 32;
        break;

      /* compressed block with fixed huffman. */
      case 1:
        while (true) {

          /* Reading fixed huffman tree entries.
           * */
          byt = access_bit<false> (&datastream[0], pos, 6);
          if (0b001100 <= byt && byt <= 0b101111) {
            byt = byt << 2 | access_bit<false> (&datastream[0], pos, 2);
            lit_val = byt - 48;
          } else if (0b110010 <= byt && byt <= 0b111111) {
            byt = byt << 3 | access_bit<false> (&datastream[0], pos, 3);
            lit_val = byt - 256;
          } else if (0b000000 <= byt && byt <= 0b001011) {
            byt = byt << 1 | access_bit<false> (&datastream[0], pos, 1);
            lit_val = byt + 256;
          } else if (0b110000 <= byt && byt <= 0b110001) {
            byt = byt << 2 | access_bit<false> (&datastream[0], pos, 2);
            lit_val = byt + 88;
          } else {
            std::cout << "zlib error: reached something bad(1)." << std::endl;
            return false;
          }

          /* Filtering the literal/length obtained.
           * */
          if (lit_val < 256) output.push_back (lit_val);
          else if (lit_val == 256) break;
          else {
            len = use_extra_len (&datastream[0], lit_val, pos);
            dist = use_extra_dist (
              &datastream[0], 
              static_cast<uint16_t> (access_bit<false> (&datastream[0], pos, 5)), 
              pos
            );

            if (output.size() < dist) {
              std::cout << "zlib error: unreachable position in buffer(1)." << std::endl;
              return false;
            }
            dist = output.size() - dist;
            for (int j = dist; j < len + dist; j++)
              output.push_back (output[j]);
          }
        }

        break;

      /* compressed block with dynamic huffman. */
      case 2: {
        hlit = access_bit (&datastream[0], pos, 5) + 257;
        hdist = access_bit (&datastream[0], pos, 5) + 1;
        hclen = access_bit (&datastream[0], pos, 4) + 4;

        if (hlit > 285 || hdist > 30 || hclen > 19) {
          std::cout << "zilb error: readed unknown information(0)." << std::endl;
          return false;
        }

        /* Construction of the length code huffman tree. 
         * */
        int i = 0;
        for (; i < hclen; i++)
          lenghts[symbol[i]] = access_bit (&datastream[0], pos, 3);
        for (; i < 19; i++)
          lenghts[symbol[i]] = 0;

        bool error;
        HuffmanTree huffman_1 (lenghts, 19, &error);
        if (error) {
          std::cout
            << "zilb error: cannot create huffman trees to length codes trees."
            << std::endl;
          return false;
        }

        /* Function to read the huffman codes. 
         * */
        HuffmanTree::iterator it1 (huffman_1);
        uint64_t lenghts_index = 0;
        while (lenghts_index < hlit + hdist) {

          /* Advance iterator. */
          while (it1.advance (access_bit (&datastream[0], pos, 1)) && !it1.finished());
          
          /* Check finding. */
          if (it1.finished ()) {
            byt = &it1;
            if (byt < 16) 
              lenghts [lenghts_index++] = byt;
            else {
              len = byt;
              byt = 0;
              switch (len) {
                case 16:
                  if (lenghts_index == 0) {
                    std::cout << "zilb error: readed unknown information(1)." << std::endl;
                    return false;
                  }
                  len = access_bit (&datastream[0], pos, 2) + 3;
                  byt = lenghts[lenghts_index - 1];
                  break;
                case 17:
                  len = access_bit (&datastream[0], pos, 3) + 3;
                  break;
                case 18:
                  len = access_bit (&datastream[0], pos, 7) + 11;
                  break;
                default:
                  std::cout << "zilb error: readed unknown information(2)." << std::endl;
                  return false;
              }
              if (lenghts_index + len > hlit + hdist) {
                std::cout << "zilb error: readed unknown information(3)." << std::endl;
                return false;
              }
              std::fill_n (lenghts.begin() + lenghts_index, len, byt);
              lenghts_index += len;
            }
            it1.go_back();
          } else {
            std::cout << "zilb error: readed unknown information(4)." << std::endl;
            return false;
          }
        }

        /* Reading and constructing of the literal/lengths huffman tree. 
         * */
        HuffmanTree huffman_2 (lenghts, hlit, &error);
        if (error) {
          std::cout 
            << "zilb error: cannot create huffman trees to literal/length trees."
            << std::endl;
          return true;
        }

        /* Reading and constructing of the distance huffman tree. 
         * */
        std::copy (lenghts.begin() + hlit, lenghts.begin() + (hlit + hdist), aux.begin());
        HuffmanTree huffman_3 (aux, hdist, &error);
        if (error) {
          std::cout 
            << "zilb error: cannot create huffman trees to distrance trees."
            << std::endl;
          return true;
        }

        /* Reading data. 
         * */
        HuffmanTree::iterator it2 (huffman_2);
        HuffmanTree::iterator it3 (huffman_3);
        while (true) {
          /* Advance iterator. */
          if (!it2.advance (access_bit (&datastream[0], pos, 1))) {  
            std::cout << "zilb error: readed unknown information(3)." << std::endl;
            return false;
          }

          /* Check finding. */
          if (it2.finished()) {
            lit_val = &it2;

            if (lit_val < 256) output.push_back (lit_val);
            else if (lit_val == 256) break;
            else {
              len = use_extra_len (&datastream[0], lit_val, pos);

              it3.go_back ();
              while (!it3.finished()) {
                if (!it3.advance (access_bit (&datastream[0], pos, 1))) {  
                  std::cout << "zilb error: readed unknown information(4)." << std::endl;
                  return false;
                }
              }
              dist = use_extra_dist (&datastream[0], &it3, pos);

              if (dist > output.size()) {
                std::cout << "zlib error: unreachable position in buffer(2)." << std::endl;
                return false;
              }
              dist = output.size() - dist;
              for (int j = dist; j < len + dist; j++)
                output.push_back (output[j]);
            }
            it2.go_back();
          }
        }

        break;
      }
      default:
        return false;
    }
    /* descomprimo. */
  } while (!bfinal);
 
  /* comparation of adler 32 code. */
  uint32_t s1 = 1, s2 = 0;
  for (auto& out: output) {
    s1 = (s1 + out) % 65521;
    s2 = (s2 + s1) % 65521;
  }
  
  int data_size = datastream.size () - 4;
  uint32_t adler = 
    datastream [data_size] << 24 | datastream [data_size + 1] << 16 | 
    datastream [data_size + 2] << 8 | datastream [data_size + 3];
  if ((s2 << 16) + s1 != adler) {
    std::cout << "zilb error: adler32 checksum missmatch." << std::endl;
    return false;
  }

  return true;
}
