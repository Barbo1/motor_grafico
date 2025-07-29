#include "../../../../headers/concepts/texture.hpp"
#include "../zlib_inflate/inflate.cpp"
#include "../zlib_inflate/read_bytes.cpp"

#include <SDL2/SDL_gamecontroller.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_stdinc.h>
#include <cstdint>
#include <fstream>
#include <iostream>

const std::size_t SHIFTING_BUFFER_SIZE = 12;
const std::size_t BUFFER_SIZE = 1 << SHIFTING_BUFFER_SIZE;

static int starting_col[7] = { 0, 4, 0, 2, 0, 1, 0 };
static int starting_row[7] = { 0, 0, 4, 0, 2, 0, 1 };
static int row_increment[7] = { 8, 8, 8, 4, 4, 2, 2 };
static int col_increment[7] = { 8, 8, 4, 4, 2, 2, 1 };

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

Texture chargePNG (SDL_Renderer* render, const std::string& path) {
  std::fstream fil(path, std::ios::binary);
  fil.open (path);
  if (!fil.is_open ()) {
    std::cout << "The file was not found." << std::endl;
    return Texture ();
  }

  fil.seekg (0, std::ios::end);
  std::size_t count = fil.tellg();
  if (count < 0) {
    fil.close ();
    std::cout << "Final position not found." << std::endl;
    return Texture ();
  }
  fil.seekg (0, std::ios::beg);

  std::vector<uint8_t> buff;
  buff.resize (count);
  if (!fil.read ((char* )buff.data (), count)) {
    std::cout << "Error while reading data." << std::endl;
    return Texture ();
  }
  fil.close ();

  uint64_t pos = 0;
  
  /* Verify signature. */
  if (buff[pos] != 0x89 || buff[pos + 1] != 0x50 || buff[pos + 2] != 0x4E || buff[pos + 3] != 0x47 || 
      buff[pos + 4] != 0x0D || buff[pos + 5] != 0x0A || buff[pos + 6] != 0x1A || buff[pos + 7] != 0x0A) {
    std::cout << "Failure in signature recognition." << std::endl;
    return Texture ();
  }
  pos += 8;

  /* Handling of IHDR chunk. */
  if (read_bytes (&buff[0], pos, 4) != 13 ||
      static_cast<uint32_t> (read_bytes (&buff[0], pos, 4)) != 0x49484452) { 
    std::cout << "IHDR chunk not founded." << std::endl;
    return Texture ();
  }

  /* Reading image parameters. */
  int width = read_bytes(&buff[0], pos, 4);
  int height = read_bytes(&buff[0], pos, 4);
  int bitdepth = read_bytes(&buff[0], pos, 1);
  int colortype  = read_bytes(&buff[0], pos, 1);
  int compmethod = read_bytes(&buff[0], pos, 1);
  int filtmethod = read_bytes(&buff[0], pos, 1);
  int intemethod = read_bytes(&buff[0], pos, 1);
  pos += 4;

  if (width == 0 || height == 0) {
    std::cout 
      << "PNG reading error: size of the total image must not be zero(width="
      << width << ", height=" << height << ")." << std::endl;
    return Texture();
  }

  if (((bitdepth - 1) & bitdepth) != 0) {
    std::cout 
      << "PNG reading error: bit depth(" << bitdepth << ") unsupported."
      << std::endl;
    return Texture();
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
        return Texture();
      }
      break;
    case 3:
      if ((bitdepth & 0x0F) == 0) {
        std::cout 
          << "PNG reading error: bit depth(" << bitdepth << ") unsupported."
          << std::endl;
        return Texture();
      }
      sampledepth = 8;
    case 0:
      if ((bitdepth & 0x1F) == 0) {
        std::cout 
          << "PNG reading error: bit depth(" << bitdepth << ") unsupported."
          << std::endl;
        return Texture();
      }
      channels = 1;
      break;
    default: 
      std::cout 
        << "PNG reading error: color type(" << colortype << ") unsupported."
        << std::endl;
      return Texture();
  }

  if (compmethod != 0) {
    std::cout 
      << "PNG reading error: descompression method(code: " << compmethod << ") not recognized."
      << std::endl;
    return Texture();
  }

  if (filtmethod != 0) {
    std::cout 
      << "PNG reading error: filter method(code: " << filtmethod << ") not recognized."
      << std::endl;
    return Texture();
  }

  if (intemethod > 2) {
    std::cout 
      << "PNG reading error: interlace method(code: " << intemethod << ") not recognized."
      << std::endl;
    return Texture();
  }

  /* Palette information. */
  std::vector<SDL_Color> palette;
  palette.reserve (15);
  int palette_count = 0;
  int plet_chnk_count = 0;

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
  std::vector<uint8_t> idat_data;
  idat_data.reserve (height * width);

  /* Reading the rest of the file. */
  bool looping = true;
  uint32_t info_lenght;
  uint64_t aux;
  while (pos < buff.size() && looping) {
    info_lenght = read_bytes(&buff[0], pos, 4);
    if (pos + info_lenght > buff.size ()) {
      std::cout 
        << "PNG reading error: size of block greater than known information."
        << std::endl;
      return Texture ();
    }

    switch (read_bytes(&buff[0], pos, 4)) {

      /* PLET */
      case 0x504C5445:
        if (colortype == 4 || colortype == 0) { 
          std::cout 
            << "PNG reading error: PLTE chunk present when it must not."
            << std::endl;
          return Texture ();
        }

        palette_count = info_lenght / 3;
        if (plet_chnk_count > 0 || info_lenght % 3 != 0 || palette_count > 1 << sampledepth) {
          std::cout 
            << "PNG reading error: PLTE chunk has an unreadable format or gives contradictory"
            << " information due the currently obtained."
            << std::endl;
          return Texture ();
        }
        plet_chnk_count++;

        palette.resize (palette_count);
        for (int i = 0; i < palette_count; i++) {
          palette[i] = {
            .r = buff[pos++],
            .g = buff[pos++],
            .b = buff[pos++],
            .a = 255,
          };
        }
        break;
  
      /* IDAT */
      case 0x49444154:
        idat_data.insert (
          idat_data.end (), 
          buff.begin() + pos, 
          buff.begin() + (pos + info_lenght)
        );
        pos += info_lenght;
        break;

      /* tRNS */
      case 0x74524E53:
        has_trns = true;
        switch (colortype) {
          case 0:
            color = read_bytes(&buff[0], pos, 2);
            bkgc = { .r = color, .g = color, .b = color, .a = 255 };
            break;
          case 2:
            bkgc = { 
              .r = (Uint8)read_bytes(&buff[0], pos, 2), 
              .g = (Uint8)read_bytes(&buff[0], pos, 2),
              .b = (Uint8)read_bytes(&buff[0], pos, 2),
              .a = 255
            };
            break;
          case 3:
            if (palette_count < info_lenght) {
              std::cout 
                << "PNG reading error: number of alpha entries in tRNS exceeded PLTE colors."
                << std::endl;
              return Texture ();
            }

            for (int i = 0; i < info_lenght; i++)
              palette[i].a = buff[pos++];
            break;
          default:
            std::cout 
              << "PNG reading error: tRNS must not be defined for color type " 
              << colortype << "."
              << std::endl;
            return Texture ();
        }

        break;
      
      /* IEND */
      case 0x49454E44:
        looping = false;
        break;

      /* The unused chunks are disregarded. */
      default:
        pos += info_lenght;
        break;
    }
    /* CRC no usado actualmente(lo desprecio). */
    pos += 4;
  }

  /* Descompression of IDAT field. */
  std::vector<uint8_t> output;
  output.reserve(idat_data.size());
  if (!inflate(idat_data, output))
    return Texture();

  /* Interlace changing. */
  SDL_Point center = SDL_Point {.x = width / 2, .y = height / 2}; 
  uint64_t many, scanline, bpp, pixels_pos;

  Texture tex = Texture ();

  Uint32* pixels = new Uint32[height * width];
  SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
  double sample_scale = 255.f / ((1 << sampledepth) - 1);

  auto get_new_pixel = [&] () {
    return (Uint8)std::roundl (sample_scale * access_bit_png (&output[0], pos, bitdepth));
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
            for (int i = bpp + many, j = many; i < scanline + many; i++, j++)
              output[i] += output[j];
            break;
          case 2:
            if (many > scanline)
              for (int i = many, j = many - scanline - 1; i < scanline + many; i++, j++)
                output[i] += output[j];
            break;
          case 3:
            if (many > scanline) {
              int i = many, j = many - scanline - 1;
              for (; i < many + bpp; i++, j++)
                output[i] += output[j] / 2;
              for (int k = many; i < scanline + many; i++, j++, k++)
                output[i] += (output[j] + output[k]) / 2;
            } else {
              /* mismo caso que en el 1 */
              for (int i = bpp + many, j = many; i < scanline + many; i++, j++)
                output[i] += output[j] / 2;
            }
            break;
          case 4:
            /* evaluo si es la primera linea. */
            if (many > scanline) {
              int i = many, j = many - scanline - 1;
              for (; i < many + bpp; i++, j++)
                output[i] += output[j];
              for (int k = many, l = many - scanline - 1; i < scanline + many; i++, j++, k++, l++)
                output[i] += paeth_predictor (output[k], output[j], output[l]);
            } else {
              /* mismo caso que en el 1. */
              for (int i = bpp + many, j = many; i < scanline + many; i++, j++)
                output[i] += output[j];
            }

            break;
          default:
            delete [] pixels;
            std::cout 
              << "PNG reading error: filter type " << aux << " unknown."
              << std::endl;
            return Texture ();
        }
        many += scanline;
      }

      pos = 8;
      pixels_pos = 0;
      switch (colortype) {
        case 6:
          for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
              Uint8 colorr = get_new_pixel ();
              Uint8 colorg = get_new_pixel ();
              Uint8 colorb = get_new_pixel ();
              Uint8 colora = get_new_pixel ();
              pixels[pixels_pos++] = SDL_MapRGBA (format, colorr, colorg, colorb, colora);
            }
            to_the_next_byte ();
          }
          break;
        case 4:
          for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
              Uint8 color = get_new_pixel ();
              Uint8 alpha = get_new_pixel ();
              pixels[pixels_pos++] = SDL_MapRGBA (format, color, color, color, alpha);
            }
            to_the_next_byte ();
          }
          break;
        case 3:
          if (plet_chnk_count != 1)  {
            std::cout << "PNG error: PLTE chunk not founded when needed." << std::endl;
            delete [] pixels;
            return Texture ();
          }
          for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
              uint64_t plet_pos = access_bit_png (&output[0], pos, bitdepth);
              if (plet_pos >= palette.size ()) {
                std::cout << "PNG error: readed PLTE index of unknown color." << std::endl;
                delete [] pixels;
                return Texture ();
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
              Uint8 colorr = get_new_pixel ();
              Uint8 colorg = get_new_pixel ();
              Uint8 colorb = get_new_pixel ();
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
              color = get_new_pixel ();
              pixels[pixels_pos++] = SDL_MapRGBA (
                format, color, color, color, (has_trns && bkgc.r == color ? 0 : 255)
              );
            }
            to_the_next_byte ();
          }
          break;
        default:
          delete [] pixels;
          std::cout << "color type not found." << std::endl;
          return Texture();
      }
      break;

    /* Adam7 de-interlace. */
    case 1: {

      int itr, itc, h, w, total;
      auto current_offset = output.begin ();
      bpp = 1 + ((bitdepth * channels - 1) >> 3);

      std::vector<uint8_t> pass;
      pass.reserve ((height + 1) * width >> 1);
      for (int pass_index = 0; pass_index < 7; pass_index++) {
        many = 0;

        w = 1 + (width - starting_col[pass_index] - 1) / col_increment[pass_index];
        h = 1 + (height - starting_row[pass_index] - 1) / row_increment[pass_index];
        scanline = 1 + ((bitdepth * channels * w - 1) >> 3);

        total = h * (1 + scanline);
        pass.clear ();
        pass.insert (pass.begin (), current_offset, current_offset + total);
        current_offset += total;

        /* Apply de-filtering algorithm. */
        many = 0;
        while (many < pass.size()) {
          aux = pass[many++];
          switch (aux) {
            case 0: 
              break;
            case 1:
              for (int i = bpp + many, j = many; i < scanline + many; i++, j++)
                pass[i] += pass[j];
              break;
            case 2:
              if (many > scanline)
                for (int i = many, j = many - scanline - 1; i < scanline + many; i++, j++)
                  pass[i] += pass[j];
              break;
            case 3:
              if (many > scanline) {
                int i = many, j = many - scanline - 1;
                for (; i < many + bpp; i++, j++)
                  pass[i] += pass[j] / 2;
                for (int k = many; i < scanline + many; i++, j++, k++)
                  pass[i] += (pass[j] + pass[k]) / 2;
              } else {
                /* mismo caso que en el 1 */
                for (int i = bpp + many, j = many; i < scanline + many; i++, j++)
                  pass[i] += pass[j] / 2;
              }
              break;
            case 4:
              /* evaluo si es la primera linea. */
              if (many > scanline) {
                int i = many, j = many - scanline - 1;
                for (; i < many + bpp; i++, j++)
                  pass[i] += pass[j];
                for (int k = many, l = many - scanline - 1; i < scanline + many; i++, j++, k++, l++)
                  pass[i] += paeth_predictor (pass[k], pass[j], pass[l]);
              } else {
                /* mismo caso que en el 1. */
                for (int i = bpp + many, j = many; i < scanline + many; i++, j++)
                  pass[i] += pass[j];
              }

              break;
            default:
              delete [] pixels;
              std::cout 
                << "PNG reading error: filter type " << aux << " unknown."
                << std::endl;
              return Texture();
          }
          many += scanline;
        }

        auto get_new_pixel = [&] () {
          return (Uint8)std::roundl (sample_scale * access_bit_png (&pass[0], pos, bitdepth));
        };

        auto to_the_next_byte = [&] () { pos += 8 + (~(pos - 1) & 7); };

        pos = 8;
        switch (colortype) {
          case 6:
            for (int i = starting_row[pass_index]; i < height; i += row_increment[pass_index]) {
              for (int j = starting_col[pass_index]; j < width; j += col_increment[pass_index]) {
                Uint8 colorr = get_new_pixel ();
                Uint8 colorg = get_new_pixel ();
                Uint8 colorb = get_new_pixel ();
                Uint8 colora = get_new_pixel ();
                pixels[i*width + j] = SDL_MapRGBA (format, colorr, colorg, colorb, colora);
              }
              to_the_next_byte ();
            }
            break;
          case 4:
            for (int i = starting_row[pass_index]; i < height; i += row_increment[pass_index]) {
              for (int j = starting_col[pass_index]; j < width; j += col_increment[pass_index]) {
                Uint8 color = get_new_pixel ();
                Uint8 alpha = get_new_pixel ();
                pixels[i*width + j] = SDL_MapRGBA (format, color, color, color, alpha);
              }
              to_the_next_byte ();
            }
            break;
          case 3:
            if (plet_chnk_count != 1)  {
              delete [] pixels;
              std::cout << "PNG error: PLTE chunk not founded when needed." << std::endl;
              return Texture ();
            }
            for (int i = starting_row[pass_index]; i < height; i += row_increment[pass_index]) {
              for (int j = starting_col[pass_index]; j < width; j += col_increment[pass_index]) {
                uint64_t plet_pos = access_bit_png (&pass[0], pos, bitdepth);
                if (plet_pos >= palette.size ()) {
                  std::cout << "PNG error: readed PLTE index of unknown color." << std::endl;
                  delete [] pixels;
                  return Texture ();
                }
                pixels[i*width + j] = SDL_MapRGBA (
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
              for (int j = starting_col[pass_index]; j < width; j += col_increment[pass_index]) {
                Uint8 colorr = get_new_pixel ();
                Uint8 colorg = get_new_pixel ();
                Uint8 colorb = get_new_pixel ();
                pixels[i*width + j] = SDL_MapRGBA (
                  format, colorr, colorg, colorb,
                  (has_trns && colorr == bkgc.r && colorg == bkgc.g && colorb == bkgc.b ? 0 : 255)
                );
              }
              to_the_next_byte ();
            }
            break;
          case 0:
            for (int i = starting_row[pass_index]; i < height; i += row_increment[pass_index]) {
              for (int j = starting_col[pass_index]; j < width; j += col_increment[pass_index]) {
                color = get_new_pixel ();
                pixels[i*width + j] = SDL_MapRGBA (
                  format, color, color, color, (has_trns && bkgc.r == color ? 0 : 255)
                );
              }
              to_the_next_byte ();
            }
            break;
          default:
            delete [] pixels;
            std::cout << "color type not found." << std::endl;
            return Texture();
        }
      }

      break;
    }
  }
  SDL_FreeFormat(format);

  tex = Texture (render, height, width, center, pixels);
  delete [] pixels;
  return tex;
}
