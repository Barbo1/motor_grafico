#pragma once

#include "./huffman_tree.cpp"
#include "./access_bit.cpp"

#include <iostream>
#include <cstdint>
#include <array>
#include <vector>

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

/* Function that takes the length code, and use it to read and return the lenght obtained. 
 * */
uint16_t use_extra_len (uint8_t* iter, uint16_t lit_val, uint64_t& i) {
  uint16_t extra, count_extra;
  lit_val -= 257;

  if (lit_val > 28)
    std::cout << "Problem reading lit_val" << std::endl;
  else if (lit_val == 28)
    return 258;

  count_extra = lenght_code_extras[lit_val];
  extra = 0;
  if (count_extra > 0) 
    extra = access_bit (iter, i, count_extra);

  return extra + lenght_code_lenghts[lit_val];
}

/* Function that takes the distance code, and use it to read and return the distance obtained. 
 * */
uint16_t use_extra_dist (uint8_t* iter, uint16_t distc, uint64_t& i) {
  uint16_t extra, count_extra;
  if (distc > 29)
    std::cout << "Problem reading distc" << std::endl;

  count_extra = distance_code_extras[distc];
  
  extra = 0;
  if (count_extra != 0) 
    extra = access_bit (iter, i, count_extra);

  return extra + distance_code_lenghts[distc];
}

/* Inflate algorithm, following the specification of RFC-1951. */
bool zlib_discompression (std::vector<uint8_t>& datastream, std::vector<uint8_t>& output) {
  uint64_t pos = 16;
 
  /* CMF and FLG comparation. */
  uint16_t cmf_flg = datastream[0] << 8 | datastream[1];
  if ((cmf_flg & 0x0F00) != 0x0800 || (cmf_flg % 31) != 0) {
    std::cout << "zilb error: cmf or flg error found." << std::endl;
    return false;
  }

  /* windos size comparation. */
  int window_size = cmf_flg & 0x00FF;
  window_size = (window_size & 0x000F) << 4 || (window_size & 0x00F0) >> 4;
  window_size += 8;
  if (window_size > 15) {
    std::cout << "zilb error: window size over 32K bytes." << std::endl;
    return false;
  }
  if ((cmf_flg & 0x0020)) pos += 32;

  /* begining of the iterations. */
  bool bfinal; 
  uint64_t dist, len;
  uint64_t lit_val, hlit, hdist, hclen, byt;
  std::vector<uint8_t> lenghts;

  do {
    bfinal = access_bit (&datastream[0], pos, 1);
    switch (access_bit (&datastream[0], pos, 2)) {

      /* non-compressed block. */
      case 0:
        std::cout << "tipo 0" << std::endl;
        pos += ((pos & 7) > 0) * (8 - (pos & 7));
        lit_val = access_bit (&datastream[0], pos, 16);
        pos += 16;
        len = pos >> 3;
        output.insert (output.end (), datastream.begin () + len, datastream.begin () + len + lit_val);
        break;

      /* compressed block with fixed huffman. */
      case 1:
        std::cout << "tipo 1" << std::endl;
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
            dist = use_extra_dist (&datastream[0], (uint16_t)access_bit<false> (&datastream[0], pos, 5), pos);

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
        std::cout << "tipo 2" << std::endl;
        hlit = access_bit (&datastream[0], pos, 5) + 257;
        hdist = access_bit (&datastream[0], pos, 5) + 1;
        hclen = access_bit (&datastream[0], pos, 4) + 4;
        std::cout << "hlit:" << hlit << std::endl;
        std::cout << "hdist:" << hdist << std::endl;
        std::cout << "hclen:" << hclen << std::endl;

        lenghts.clear ();
        lenghts.reserve (19);
        for (int j = 0 ; j < hclen; j++)
          lenghts.push_back (access_bit (&datastream[0], pos, 3));
        lenghts.insert (lenghts.end (), 19 - hclen, 0);

        /* Construction of the length code huffman tree. 
         * */
        bool error;
        std::cout << std::endl << "muestro arreglo(" << lenghts.size () << "): " << std::endl;
        for (auto& l: lenghts)
          std::cout << (int)l << ", ";
        std::cout << std::endl << std::endl;
        HuffmanTree huffman_1 (lenghts, 1, &error);
        if (error) {
          std::cout << "error creating huffman: codes overlapse(0)." << std::endl;
          return false;
        }
        HuffmanTree::iterator it1 (huffman_1);

        /* Reading distances from the array. 
         * */
        lenghts.clear ();
        lenghts.reserve (hlit + hdist);
        while (lenghts.size () < hlit + hdist) {
          if (!it1.advance (access_bit (&datastream[0], pos, 1))) {
            std::cout << "zilb error: readed unknown information(1)." << std::endl;
            return false;
          }
          if (it1.finished()) {
            byt = &it1;
            if (byt < 16)
              lenghts.push_back (byt);
            else if (byt == 16)
              lenghts.insert (lenghts.end(), access_bit (&datastream[0], pos, 2) + 3, lenghts.back ());
            else if (byt == 17)
              lenghts.insert (lenghts.end(), access_bit (&datastream[0], pos, 3) + 3, 0);
            else if (byt == 18)
              lenghts.insert (lenghts.end(), access_bit (&datastream[0], pos, 7) + 11, 0);
            else {
              std::cout << "zilb error: readed unknown information(2)." << std::endl;
              return false;
            }
            it1.go_back();
          }
        }
        
        std::cout << std::endl << "otros dos arreglo:" << std::endl;
        int m = 0;
        for (; m < hlit; m++)
          std::cout << (int)lenghts[m] << ", ";
        std::cout << std::endl << std::endl;
        for (; m < hdist + hlit; m++)
          std::cout << (int)lenghts[m] << ", ";
        std::cout << std::endl;
        std::cout << "creo uno" << std::endl;
        
        /* Construction of the distance huffman tree. 
         * */
        HuffmanTree huffman_3 (
          std::vector<uint8_t> (lenghts.begin() + hlit, lenghts.begin () + hlit + hdist),
          0,
          &error
        );
        if (error) {
          std::cout << "error creating huffman: codes overlapse(2)." << std::endl;
          return false;
        }

        /* Construction of the literal/lenghts huffman tree. 
         * */
        HuffmanTree huffman_2 (
          std::vector<uint8_t> (lenghts.begin(), lenghts.begin() + hlit),
          0,
          &error
        );
        if (error) {
          std::cout << "error creating huffman: codes overlapse(1)." << std::endl;
          return false;
        }

        /* lectura de datos. */
        HuffmanTree::iterator it2 (huffman_2);
        HuffmanTree::iterator it3 (huffman_3);
        while (true) {
          /* avanzo iterador de literales. */
          if (!it2.advance (access_bit (&datastream[0], pos, 1))) {  
            std::cout << "zilb error: readed unknown information(3)." << std::endl;
            return false;
          }

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
              dist = &it3;
              dist = use_extra_dist (&datastream[0], dist, pos);

              /* copy elements. */
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
        std::cout << "zlib error: block type unrecognized" << std::endl;
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
