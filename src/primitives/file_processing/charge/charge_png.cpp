#include "../../../../headers/concepts/visualizer.hpp"
#include "../../../../headers/primitives/file_processing.hpp"
#include "../zlib_inflate/inflate.cpp"

#include <SDL2/SDL_stdinc.h>
#include <emmintrin.h>
#include <immintrin.h>
#include <cstdint>
#include <iostream>

static int starting_col[7] = { 0, 4, 0, 2, 0, 1, 0 };
static int starting_row[7] = { 0, 0, 4, 0, 2, 0, 1 };
static int row_increment[7] = { 8, 8, 8, 4, 4, 2, 2 };
static int col_increment[7] = { 8, 8, 4, 4, 2, 2, 1 };
static int row_increment_sh[7] = { 3, 3, 3, 2, 2, 1, 1 };
static int col_increment_sh[7] = { 3, 3, 2, 2, 1, 1, 0 };

uint8_t paeth_predictor (int16_t left, int16_t above, int16_t diagonal) {
  int16_t p = left + above - diagonal;
  int16_t pa = std::abs (p - left);
  int16_t pb = std::abs (p - above);
  int16_t pc = std::abs (p - diagonal);

  if (pa <= pb && pa <= pc) 
    return static_cast<uint8_t>(left);
  else if (pb <= pc) 
    return static_cast<uint8_t>(above);
  else 
    return static_cast<uint8_t>(diagonal);
}

Uint32* charging_PNG_to_memory (const std::string& path, int & width, int & height) {
  int error;
  SequentialFileReader fil = SequentialFileReader(path, &error);
  if (error != 0) {
    std::cout << "The file was not found." << std::endl;
    return nullptr;
  }
  
  /* Verify signature. */
  if (fil.read64() != 0x89504E470D0A1A0A) {
    std::cout << "Failure in signature recognition." << std::endl;
    return nullptr;
  }

  /* Handling of IHDR chunk. */
  if (fil.read32() != 13 || fil.read32() != 0x49484452) { 
    std::cout << "IHDR chunk not founded." << std::endl;
    return nullptr;
  }

  /* Reading image parameters. */
  width = fil.read32();
  height = fil.read32();
  int bitdepth = fil.read8();
  int colortype  = fil.read8();
  int compmethod = fil.read8();
  int filtmethod = fil.read8();
  int intemethod = fil.read8();
  fil.skip (4);

  if (width == 0 || height == 0) {
    std::cout 
      << "PNG reading error: size of the total image must not be zero(width="
      << width << ", height=" << height << ")." << std::endl;
    return nullptr;
  }

  if (((bitdepth - 1) & bitdepth) != 0) {
    std::cout 
      << "PNG reading error: bit depth(" << bitdepth << ") unsupported."
      << std::endl;
    return nullptr;
  }

  int sampledepth = bitdepth;
  int channels = 0;
  switch (colortype) {
    case 6:
      channels = 4;
      goto EVAL_CHAN;
    case 2: 
      channels = 3;
      goto EVAL_CHAN;
    case 4: 
      channels = 2;
      EVAL_CHAN:
      if ((bitdepth & 0x18) == 0) {
        std::cout 
          << "PNG reading error: bit depth(" << bitdepth << ") unsupported."
          << std::endl;
        return nullptr;
      }
      break;
    case 3:
      if ((bitdepth & 0x0F) == 0) {
        std::cout 
          << "PNG reading error: bit depth(" << bitdepth << ") unsupported."
          << std::endl;
        return nullptr;
      }
      sampledepth = 8;
    case 0:
      if ((bitdepth & 0x1F) == 0) {
        std::cout 
          << "PNG reading error: bit depth(" << bitdepth << ") unsupported."
          << std::endl;
        return nullptr;
      }
      channels = 1;
      break;
    default: 
      std::cout 
        << "PNG reading error: color type(" << colortype << ") unsupported."
        << std::endl;
        return nullptr;
  }

  if (compmethod != 0) {
    std::cout 
      << "PNG reading error: descompression method(code: " << compmethod << ") not recognized."
      << std::endl;
    return nullptr;
  }

  if (filtmethod != 0) {
    std::cout 
      << "PNG reading error: filter method(code: " << filtmethod << ") not recognized."
      << std::endl;
    return nullptr;
  }

  if (intemethod > 2) {
    std::cout 
      << "PNG reading error: interlace method(code: " << intemethod << ") not recognized."
      << std::endl;
    return nullptr;
  }

  /* Palette information. */
  std::vector<SDL_Color> palette;
  palette.reserve (15);
  uint32_t palette_count = 0;
  uint32_t plet_chnk_count = 0;

  /* background color. */
  Uint8 color;
  bool has_trns = false;
  SDL_Color bkgc = {
    .r = 0,
    .g = 0,
    .b = 0,
    .a = 0
  };  

  /* IDAT information. */
  std::vector<uint8_t> idat_new;
  std::vector<uint8_t> idat_data;
  idat_data.reserve (height * width);

  /* Reading the rest of the file. */
  bool looping = true;
  uint32_t info_lenght;
  while (!fil.finished() && looping) {
    info_lenght = fil.read32();
    if (!fil.spare(info_lenght)) {
      std::cout 
        << "PNG reading error: size of block greater than known information."
        << std::endl;
      return nullptr;
    }

    switch (fil.read32()) {

      /* PLET */
      case 0x504C5445:
        if (colortype == 4 || colortype == 0) { 
          std::cout 
            << "PNG reading error: PLTE chunk present when it must not."
            << std::endl;
          return nullptr;
        }

        palette_count = info_lenght / 3;
        if (plet_chnk_count > 0 || info_lenght % 3 != 0 || palette_count > static_cast<uint32_t>(1 << sampledepth)) {
          std::cout 
            << "PNG reading error: PLTE chunk has an unreadable format or gives contradictory"
            << " information due the currently obtained."
            << std::endl;
          return nullptr;
        }
        plet_chnk_count++;

        palette.resize (palette_count);
        for (uint32_t i = 0; i < palette_count; i++) {
          palette[i] = {
            .r = fil.read8(),
            .g = fil.read8(),
            .b = fil.read8(),
            .a = 255,
          };
        }
        break;
  
      /* IDAT */
      case 0x49444154:
        idat_new = fil.get_vector(info_lenght);
        idat_data.insert(idat_data.end(), idat_new.begin(), idat_new.end());
        break;

      /* tRNS */
      case 0x74524E53:
        has_trns = true;
        switch (colortype) {
          case 0:
            color = fil.read16();
            bkgc = { .r = color, .g = color, .b = color, .a = 255 };
            break;
          case 2:
            bkgc = { 
              .r = (Uint8)fil.read16(), 
              .g = (Uint8)fil.read16(),
              .b = (Uint8)fil.read16(),
              .a = 255
            };
            break;
          case 3:
            if (palette_count < info_lenght) {
              std::cout 
                << "PNG reading error: number of alpha entries in tRNS exceeded PLTE colors."
                << std::endl;
              return nullptr;
            }

            for (uint32_t i = 0; i < info_lenght; i++)
              palette[i].a = fil.read8();
            break;
          default:
            std::cout 
              << "PNG reading error: tRNS must not be defined for color type " 
              << colortype << "."
              << std::endl;
            return nullptr;
        }

        break;
      
      /* IEND */
      case 0x49454E44:
        looping = false;
        break;

      /* The unused chunks are disregarded. */
      default:
        fil.skip (info_lenght);
        break;
    }
    /* CRC not supported. */
    fil.skip (4);
  }

  /* Descompression of IDAT field. */
  std::vector<uint8_t> output;
  output.reserve(idat_data.size());
  if (!inflate(idat_data, output))
    return nullptr;

  /* Interlace changing. */
  uint64_t many, scanline, bpp, pixels_pos, aux, pos;
  double sample_scale = 255.f / ((1 << sampledepth) - 1);

  /* Pixels allocation. */
  Uint32* pixels = new Uint32[height * width];
  SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);

  /* Changes to output position. */
  auto get_new_pixel = [&] (const std::vector<uint8_t>& output) {
    return std::roundl (sample_scale * access_bit_png (&output[0], pos, bitdepth));
  };
  auto to_the_next_byte = [&] () { pos += 8 + (~(pos - 1) & 7); };

  switch (intemethod) {

    /* none de-interlace. */
    case 0:
      many = 0;
      scanline = bitdepth * channels;
      bpp = 1 + ((scanline - 1) >> 3);
      scanline = 1 + ((scanline * width - 1) >> 3);

      /* Apply de-filtering algorithm. */
      while (many < output.size()) {
        aux = output[many++];
        switch (aux) {
          case 0: 
            break;
          case 1:
            for (uint32_t i = bpp + many, j = many; i < scanline + many; i++, j++)
              output[i] += output[j];
            break;
          case 2:
            if (many > scanline) {
              uint32_t i = many, j = many - scanline - 1;
              for (uint32_t count = 0; count < (scanline >> 4); count++, j+=16, i+=16) {
                __m128i op1 = _mm_loadu_si128 ((__m128i*)(&output[i]));
                __m128i op2 = _mm_loadu_si128 ((__m128i*)(&output[j]));
                _mm_storeu_si128((__m128i*)(&output[i]), _mm_add_epi8(op1, op2));
              }
              for (uint32_t count = 0; count < (scanline & 0b1111); count++, i++, j++) {
                output[i] += output[j];
              }
            }
            break;
          case 3:
            if (many > scanline) {
              uint32_t i = many, j = many - scanline - 1;
              for (; i < many + bpp; i++, j++)
                output[i] += output[j] >> 1;
              for (int k = many; i < (scanline - bpp) >> 4; i+=16, j+=16, k+=16) {
                __m128i op1 = _mm_loadu_si128 ((__m128i*)(&output[i]));
                __m128i op2 = _mm_loadu_si128 ((__m128i*)(&output[j]));
                __m128i op3 = _mm_loadu_si128 ((__m128i*)(&output[k]));
                _mm_storeu_si128((__m128i*)(&output[i]), _mm_add_epi8(op1, _mm_avg_epu8(op2, op3)));
              }
              for (int k = many; i < scanline + many; i++, j++, k++)
                output[i] += (output[j] + output[k]) / 2;
            } else {
              /* mismo caso que en el 1 */
              for (uint32_t i = bpp + many, j = many; i < scanline + many; i++, j++)
                output[i] += output[j] / 2;
            }
            break;
          case 4:
            /* evaluo si es la primera linea. */
            if (many > scanline) {
              uint32_t i = many, j = many - scanline - 1;
              for (; i < many + bpp; i++, j++)
                output[i] += output[j];
              for (int k = many, l = many - scanline - 1; i < scanline + many; i++, j++, k++, l++)
                output[i] += paeth_predictor (output[k], output[j], output[l]);
            } else {
              /* mismo caso que en el 1. */
              for (uint32_t i = bpp + many, j = many; i < scanline + many; i++, j++)
                output[i] += output[j];
            }

            break;
          default:
            std::cout << "PNG reading error: filter type " << aux << " unknown." << std::endl;
            SDL_FreeFormat(format);
            delete [] pixels;
            return nullptr;
        }
        many += scanline;
      }

      pos = 8;
      pixels_pos = 0;
      switch (colortype) {
        case 6:
          for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
              Uint8 colorr = get_new_pixel (output);
              Uint8 colorg = get_new_pixel (output);
              Uint8 colorb = get_new_pixel (output);
              Uint8 colora = get_new_pixel (output);
              pixels[pixels_pos++] = SDL_MapRGBA (format, colorr, colorg, colorb, colora);
            }
            to_the_next_byte ();
          }
          break;
        case 4:
          for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
              Uint8 color = get_new_pixel (output);
              Uint8 alpha = get_new_pixel (output);
              pixels[pixels_pos++] = SDL_MapRGBA (format, color, color, color, alpha);
            }
            to_the_next_byte ();
          }
          break;
        case 3:
          if (plet_chnk_count != 1)  {
            SDL_FreeFormat(format);
            delete [] pixels;
            std::cout << "PNG error: PLTE chunk not founded when needed." << std::endl;
            return nullptr;
          }
          for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
              uint64_t plet_pos = access_bit_png (&output[0], pos, bitdepth);
              if (plet_pos >= palette.size ()) {
                SDL_FreeFormat(format);
                delete [] pixels;
                std::cout << "PNG error: readed PLTE index of unknown color." << std::endl;
                return nullptr;
              }
              pixels[pixels_pos++] = SDL_MapRGBA (
                format,
                palette[plet_pos].r,
                palette[plet_pos].g,
                palette[plet_pos].b,
                palette[plet_pos].a
              );
            }
            to_the_next_byte ();
          }
          break;
        case 2:
          for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
              Uint8 colorr = get_new_pixel (output);
              Uint8 colorg = get_new_pixel (output);
              Uint8 colorb = get_new_pixel (output);
              pixels[pixels_pos++] = SDL_MapRGBA (
                format, colorr, colorg, colorb,
                (has_trns && colorr == bkgc.r && colorg == bkgc.g && colorb == bkgc.b ? 0 : 255)
              );
            }
            to_the_next_byte ();
          }
          break;
        case 0:
          for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
              color = get_new_pixel (output);
              pixels[pixels_pos++] = SDL_MapRGBA (
                format, color, color, color, (has_trns && bkgc.r == color ? 0 : 255)
              );
            }
            to_the_next_byte ();
          }
          break;
        default:
          SDL_FreeFormat(format);
          delete [] pixels;
          std::cout << "color type not found." << std::endl;
          return nullptr;
      }
      break;

    /* Adam7 de-interlace. */
    case 1: {

      int w, total;
      auto current = output.begin ();
      bpp = 1 + ((bitdepth * channels - 1) >> 3);

      std::vector<uint8_t> pass;
      pass.reserve ((height + 1) * width);
      for (int pass_index = 0; pass_index < 7; pass_index++) {
        many = 0;

        w = 1 + ((width - starting_col[pass_index] - 1) >> col_increment_sh[pass_index]);
        scanline = 1 + ((bitdepth * channels * w - 1) >> 3);
        total = (1 + ((height - starting_row[pass_index] - 1) >> row_increment_sh[pass_index])) * (1 + scanline);

        pass.clear ();
        pass.insert (pass.begin (), current, current + total);
        current += total;

        /* Apply de-filtering algorithm. */
        many = 0;
        while (many < pass.size()) {
          aux = pass[many++];
          switch (aux) {
            case 0: 
              break;
            case 1:
              for (uint32_t i = bpp + many, j = many; i < scanline + many; i++, j++)
                pass[i] += pass[j];
              break;
            case 2:
              if (many > scanline) {
                uint32_t i = many, j = many - scanline - 1;
                for (uint32_t count = 0; count < (scanline >> 4); count++, j+=16, i+=16) {
                  __m128i op1 = _mm_loadu_si128 ((__m128i*)(&pass[i]));
                  __m128i op2 = _mm_loadu_si128 ((__m128i*)(&pass[j]));
                  _mm_storeu_si128((__m128i*)(&pass[i]), _mm_add_epi8(op1, op2));
                }
                for (uint32_t count = 0; count < (scanline & 0b1111); count++, i++, j++) {
                  pass[i] += pass[j];
                }
              }
              break;
            case 3:
              if (many > scanline) {
                uint32_t i = many, j = many - scanline - 1;
                for (; i < many + bpp; i++, j++)
                  pass[i] += pass[j] >> 1;
                for (int k = many; i < (scanline - bpp) >> 4; i+=16, j+=16, k+=16) {
                  __m128i op1 = _mm_loadu_si128 ((__m128i*)(&pass[i]));
                  __m128i op2 = _mm_loadu_si128 ((__m128i*)(&pass[j]));
                  __m128i op3 = _mm_loadu_si128 ((__m128i*)(&pass[k]));
                  _mm_storeu_si128((__m128i*)(&pass[i]), _mm_add_epi8(op1, _mm_avg_epu8(op2, op3)));
                }
                for (int k = many; i < scanline + many; i++, j++, k++)
                  pass[i] += (pass[j] + pass[k]) / 2;
              } else {
                /* mismo caso que en el 1 */
                for (uint32_t i = bpp + many, j = many; i < scanline + many; i++, j++)
                  pass[i] += pass[j] / 2;
              }
              break;
            case 4:
              /* evaluo si es la primera linea. */
              if (many > scanline) {
                uint32_t i = many, j = many - scanline - 1;
                for (; i < many + bpp; i++, j++)
                  pass[i] += pass[j];
                for (uint32_t k = many, l = many - scanline - 1; i < scanline + many; i++, j++, k++, l++)
                  pass[i] += paeth_predictor (pass[k], pass[j], pass[l]);
              } else {
                /* mismo caso que en el 1. */
                for (uint32_t i = bpp + many, j = many; i < scanline + many; i++, j++)
                  pass[i] += pass[j];
              }

              break;
            default:
              std::cout 
                << "PNG reading error: filter type " << aux << " unknown."
                << std::endl;
              SDL_FreeFormat(format);
              delete [] pixels;
              return nullptr;
          }
          many += scanline;
        }

        pos = 8;
        switch (colortype) {
          case 6:
            for (int i = starting_row[pass_index]; i < height; i += row_increment[pass_index]) {
              for (int j = i*width + starting_col[pass_index]; j < (i + 1) * width; j += col_increment[pass_index]) {
                Uint8 colorr = get_new_pixel (pass);
                Uint8 colorg = get_new_pixel (pass);
                Uint8 colorb = get_new_pixel (pass);
                Uint8 colora = get_new_pixel (pass);
                pixels[j] = SDL_MapRGBA (format, colorr, colorg, colorb, colora);
              }
              to_the_next_byte ();
            }
            break;
          case 4:
            for (int i = starting_row[pass_index]; i < height; i += row_increment[pass_index]) {
              for (int j = i*width + starting_col[pass_index]; j < (i + 1) * width; j += col_increment[pass_index]) {
                Uint8 color = get_new_pixel (pass);
                Uint8 alpha = get_new_pixel (pass);
                pixels[j] = SDL_MapRGBA (format, color, color, color, alpha);
              }
              to_the_next_byte ();
            }
            break;
          case 3:
            if (plet_chnk_count != 1)  {
              std::cout << "PNG error: PLTE chunk not founded when needed." << std::endl;
              SDL_FreeFormat(format);
              delete [] pixels;
              return nullptr;
            }
            for (int i = starting_row[pass_index]; i < height; i += row_increment[pass_index]) {
              for (int j = i*width + starting_col[pass_index]; j < (i + 1) * width; j += col_increment[pass_index]) {
                uint64_t plet_pos = access_bit_png (&pass[0], pos, bitdepth);
                if (plet_pos >= palette.size ()) {
                  SDL_FreeFormat(format);
                  delete [] pixels;
                  std::cout << "PNG error: readed PLTE index of unknown color." << std::endl;
                  return nullptr;
                }
                pixels[j] = SDL_MapRGBA (
                  format, 
                  palette[plet_pos].r, 
                  palette[plet_pos].g,
                  palette[plet_pos].b, 
                  palette[plet_pos].a
                );
              }
              to_the_next_byte ();
            }
            break;
          case 2:
            for (int i = starting_row[pass_index]; i < height; i += row_increment[pass_index]) {
              for (int j = i * width + starting_col[pass_index]; j < (i + 1) * width; j += col_increment[pass_index]) {
                Uint8 colorr = get_new_pixel (pass);
                Uint8 colorg = get_new_pixel (pass);
                Uint8 colorb = get_new_pixel (pass);
                pixels[j] = SDL_MapRGBA (
                  format, colorr, colorg, colorb,
                  (has_trns && colorr == bkgc.r && colorg == bkgc.g && colorb == bkgc.b ? 0 : 255)
                );
              }
              to_the_next_byte ();
            }
            break;
          case 0:
            for (int i = starting_row[pass_index]; i < height; i += row_increment[pass_index]) {
              for (int j = i * width + starting_col[pass_index]; j < (i + 1) * width; j += col_increment[pass_index]) {
                color = get_new_pixel (pass);
                pixels[j] = SDL_MapRGBA (
                  format, color, color, color, (has_trns && bkgc.r == color ? 0 : 255)
                );
              }
              to_the_next_byte ();
            }
            break;
          default:
            SDL_FreeFormat(format);
            delete [] pixels;
            std::cout << "color type not found." << std::endl;
            return nullptr;
        }
      }

      break;
    }
  }

  SDL_FreeFormat(format);
  return pixels;
}
