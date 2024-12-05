#pragma once

#define lp_random
#define lp_math
#include <lp_helper.h>

#include <png.h>
#include <vector>
#include <cstdio>

enum TextureType
{
	Box = 0,
	Gradient = 1,
	Grass = 2,
};

struct Pixel {
	// One byte unsigned int
    u_int8_t r, g, b, a;

	// Default constructor
	Pixel() : r(0), g(0), b(0), a(255) {}

	Pixel(u_int8_t val): r(val), g(val), b(val), a(255) {}

	Pixel(u_int8_t red, u_int8_t green, u_int8_t blue, u_int8_t alpha):
		r(red), g(green), b(blue), a(alpha) {}

	// RGBA float 0.f - 1.f
	Pixel(float red, float green, float blue, float alpha):
		r( static_cast <u_int8_t> (red * 255) ),
		g( static_cast <u_int8_t> (green * 255) ),
		b( static_cast <u_int8_t> (blue * 255) ),
		a( static_cast <u_int8_t> (alpha * 255) ) {}
};

static std::vector<uint> gen_noise(uint width, uint height)
{
	std::vector<uint> noise(width * height);

	for (uint index = 0; index < (width * height); index++)
	{
		noise[index] = random_byte();
	}

	return noise;
}

static std::vector<Pixel> box_texture(uint width, uint height, uint margin, uint mar_tex_scl = 1, uint insides_tex_scl = 1)
{
	std::vector<Pixel> pixels(width * height);

	std::vector<uint> noise_1 = gen_noise((int) width / mar_tex_scl, (int) height / mar_tex_scl);
	std::vector<uint> noise_2 = gen_noise((int) width / insides_tex_scl, (int) height / insides_tex_scl);
	std::cout << "Starting gen...\n";
	for (uint index = 0; index < (width * height); index++)
	{
		uint x = index % width;
		uint y = static_cast<int>( index / height );
		float grayscale = static_cast<float>(width - x) / static_cast<float>(width);

		u_int8_t r, g, b, a;

		// Get pixel color
		uint n_pix_i;
		float noise_val;

		// Border
		if (x < margin || x > width - margin || y < margin || y > height - margin)
		{
			n_pix_i = (unsigned int) ((x / mar_tex_scl) * (y / mar_tex_scl));
			noise_val = ((float) (noise_1[n_pix_i]) / 255);

			r = g = b = clamp(180U + (unsigned int) (noise_val*20), 180U, 200U);
		}
		// Color insides
		else
		{
			n_pix_i = (unsigned int) ((x / insides_tex_scl) * (y / insides_tex_scl));
			noise_val = ((float) (noise_2[n_pix_i]) / 255);
			r = g = b = clamp(215U + (unsigned int) (noise_val*25), 200U, 240U);
		}
		a = 255;

        pixels[index].r = r;
        pixels[index].g = g;
        pixels[index].b = b;
        pixels[index].a = a;
	}

	return pixels;
}

/*
 * Width - width of image
 * Height - height of image
 * Format - may be "RGB" or "RGBA" (with alpha channel)
 * Target Dir - directory to write image file 
*/
static int gen_image (uint width, uint height, std::string target_dir)
{
	std::string target_file(target_dir + "/" + uuid::gen_v4() + ".png");

	std::vector<Pixel> pixels = box_texture(width, height, 16U, 8U, 16U);

	// Check existing 'tmp' directory
	if (!fs::is_directory("tmp") || !fs::exists("tmp"))
	{
		fs::create_directory("tmp");
	}

	// Этот код блять сплошная обработка ошибок, плюсом этому...
	// ... libpng - прекрасен, но блять, можете называть функции и переменные...
	// ... вкладывая в них хоть какой-то блять смысл
	// ЗАЕБЛО!!!!

	// Open file for writing
	FILE* file = fopen(target_file.data(), "wb");
	if (!file)
	{
		std::cerr << "gen_image ERROR: Cannot open file for writing\n";
		return 1;
	}

	// Create png structure of libpng
	png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr)
	{
		fclose(file);
		pixels.clear();
		std::cerr << "gen_image ERROR: Cannot create png_struct pointer entity\n";
		return 1;
	}

	// Create info section of png
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
		fclose(file);
		pixels.clear();
		std::cerr << "gen_image ERROR: Cannot create png_info struct pointer entity\n";
		return 1;
	}

	if (setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
		fclose(file);
		pixels.clear();
		std::cerr << "gen_image ERROR: Error occured creating libpng write context\n";
		return 1;
	}

	png_init_io(png_ptr, file);

	// Set PNG parametres
	png_set_IHDR
	(
		png_ptr,
		info_ptr,
		width,
		height,
		8, // Bit color depth
		PNG_COLOR_TYPE_RGBA,
		PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT,
		PNG_FILTER_TYPE_DEFAULT
	);

	// Writing header to png
	png_write_info(png_ptr, info_ptr);

	// Write pixel data
	png_byte row[sizeof(png_byte)*width*4];
	for (uint line = 0; line < height; line++)
	{
		// Writing data line by line
		for (uint x_line = 0; x_line < width; x_line++)
		{
			uint index = x_line + line * height;
			Pixel* pixel = &pixels[index];
			*(row + x_line * 4 + 0) = static_cast<unsigned char>(pixel->r); // Red
			*(row + x_line * 4 + 1) = static_cast<unsigned char>(pixel->g); // Green
			*(row + x_line * 4 + 2) = static_cast<unsigned char>(pixel->b); // Blue
			*(row + x_line * 4 + 3) = static_cast<unsigned char>(pixel->a); // Alpha
			
		}
		png_write_row(png_ptr, row);
	}

	// End of writing
	png_write_end(png_ptr, NULL);

	// Relese resources
	png_destroy_write_struct(&png_ptr, &info_ptr);
	fclose(file);
	pixels.clear();

	std::cout <<  "File: " << target_file << "\n";

	// Exit code 0 - success
	return 0;
}