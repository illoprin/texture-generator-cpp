#pragma once

#define lp_random
#include <lp_helper.h>
#include <png.h>
#include <vector>
#include <cstdio>

struct Pixel {
    uint r, g, b, a;
};

// Parameter is inactive
static std::vector<Pixel> box_texture(uint width, uint height, uint margin)
{
	std::vector<Pixel> pixels(width * height);

	for (uint index = 0; index < (width * height); index++)
	{
		uint x = index % width;
		uint y = static_cast<int>( index / height );
		float grayscale = static_cast<float>(width - x) / static_cast<float>(width);

		// Box texture
		uint r, g, b, a;

		// Get pixel color
		
		// Border
		if (x < margin || x > width - margin || y < margin || y > height - margin)
		{
			r = g = b = 120 + random_uniform_unsigned(0, 30) * static_cast<uint>(ceil(random_real()));
		}
		// Noise insides
		else
		{
			r = g = b = 200 + random_uniform_unsigned(0, 54);
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

	std::vector<Pixel> pixels = box_texture(width, height, 16);

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