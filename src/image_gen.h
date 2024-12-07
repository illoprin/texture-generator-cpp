#pragma once

#include <cstdint>

#define lp_random
#define lp_math
// -- My own lib on C++ --
#include <lp_helper.h> // Some helper functions (random, uuid, math)

#include <libpng/png.h> // libpng structures
#include <vector>
#include <cstdio>


enum TextureType
{
	NoTexture = 0,
	Box = 1,
	Grass = 2,
	MetalWall = 3,
	OfficeWall = 4
};

struct Pixel {
	// One byte unsigned int per pixel
    uint8_t r, g, b, a;

	// Default constructor
	Pixel() : r(0), g(0), b(0), a(255) {}

	Pixel(uint8_t val): r(val), g(val), b(val), a(255) {}

	Pixel(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha):
		r(red), g(green), b(blue), a(alpha) {}

	// Constructor for RGBA float 0.f - 1.f
	Pixel(float red, float green, float blue, float alpha):
		r( static_cast <uint8_t> (red * 255) ),
		g( static_cast <uint8_t> (green * 255) ),
		b( static_cast <uint8_t> (blue * 255) ),
		a( static_cast <uint8_t> (alpha * 255) ) {}
};

static std::vector<unsigned int> gen_noise(unsigned int width, unsigned int height)
{
	std::vector<unsigned int> noise(width * height);

	for (unsigned int index = 0; index < (width * height); index++)
	{
		noise[index] = random_byte();
	}

	return noise;
}

static std::vector<Pixel> box_texture(unsigned int width, unsigned int height, unsigned int margin, unsigned int mar_tex_scl = 1, unsigned int insides_tex_scl = 1)
{
	std::vector<Pixel> pixels(width * height);

	std::vector<unsigned int> noise_1 = gen_noise((int) width / mar_tex_scl, (int) height / mar_tex_scl);
	std::vector<unsigned int> noise_2 = gen_noise((int) width / insides_tex_scl, (int) height / insides_tex_scl);

	// Pixel x, y
	unsigned int x, y;
	// Pixel x, y in noise space
	unsigned int n_x, n_y;
	// Pixel color 0-255
	uint8_t r, g, b, a;
	// Pixel index in grain texture
	unsigned int n_pix_i;
	// Coefficient of existence of noise
	float noise_ratio;
	for (unsigned int index = 0; index < (width * height); index++)
	{
		x = index % width;
		y = (unsigned int) (index / height);
		
		// Border
		if (x < margin || x > width - margin || y < margin || y > height - margin)
		{
			n_x = (unsigned int)(x / mar_tex_scl); n_x = n_x < 1U ? 1U : n_x;
			n_y = (unsigned int)(y / mar_tex_scl); n_y = n_y < 1U ? 1U : n_y;
			n_pix_i = n_x * n_y;
			noise_ratio = (float)noise_1[n_pix_i] / 255.f;

			r = g = b = my_math::clamp(180U + (unsigned int) (noise_ratio*20), 180U, 200U);
		}
		// Color insides
		else
		{
			
			n_x = (unsigned int)(x / insides_tex_scl); n_x = n_x < 1U ? 1U : n_x;
			n_y = (unsigned int)(y / insides_tex_scl); n_y = n_y < 1U ? 1U : n_y;
			n_pix_i = n_x * n_y;
			noise_ratio = (float)noise_2[n_pix_i] / 255.f;
			r = g = b = my_math::clamp(215U + (unsigned int) (noise_ratio*25), 200U, 240U);
		}
		a = 255; // Pixel alpha value

        pixels[index].r = r;
        pixels[index].g = g;
        pixels[index].b = b;
        pixels[index].a = a;
	}
	noise_1.clear();
	noise_2.clear();

	return pixels;
}

static std::vector<Pixel> grass_texture(unsigned int width, unsigned int height, unsigned int grain_scl = 4U)
{
	std::vector<Pixel> pixels(width * height);

	std::vector<unsigned int> grain = gen_noise((unsigned int) width / grain_scl , (unsigned int) height / grain_scl);

	// Pixel x, y
	unsigned int x, y;
	// Pixel x, y in noise space
	unsigned int n_x, n_y;
	// Pixel color 0-255
	uint8_t r, g, b, a;
	// Pixel index in grain texture
	unsigned int n_pix_i;
	// Existence coefficient of noise
	float noise_ratio;
	for (unsigned int index = 0; index < width * height; index++)
	{
		x = index % height;
		y = (unsigned int) (index / width);
		n_x = (unsigned int)(x / grain_scl); n_x = n_x < 1u ? 1u : n_x;
		n_y = (unsigned int)(y / grain_scl); n_y = n_y < 1u ? 1u : n_y;

		// Get pixel color
		n_pix_i = n_x * n_y;
		noise_ratio = (float)grain[n_pix_i] / 255.f;
		r = my_math::clamp(79U + (unsigned int)(noise_ratio * 60), 79U, 230U);
		g = my_math::clamp(135U + (unsigned int)(noise_ratio * 85), 130U, 230U);
		b = my_math::clamp(20U + (unsigned int)(noise_ratio * 50), 19U, 230U);
		a = 255;

		pixels[index].r = r;
		pixels[index].g = g;
		pixels[index].b = b;
		pixels[index].a = a;
	}
	grain.clear();

	return pixels;
}

/*
 * Width - width of image
 * Height - height of image
 * Target Dir - directory to write image file 
*/
static int gen_image (unsigned int width, unsigned int height, std::string target_dir, TextureType type)
{
	std::vector<Pixel> pixels;
	std::string file_name;

	switch (type) {
		case TextureType::Box:
			pixels = box_texture(width, height, 16U, 8U, 16U);
			file_name = "box" + std::to_string(random_uniform_unsigned(1001, 9999));
		break;

		case TextureType::Grass:
			pixels = grass_texture(width, height, 8U);
			file_name = "grass" + std::to_string(random_uniform_unsigned(1001, 9999));
		break;

		case TextureType::NoTexture:
			std::cout << "\nPlease, write currect texture type\n";
			return 1;
		break;

		default:
			pixels = box_texture(width, height, 16U, 8U, 16U);
			file_name = "box" + std::to_string(random_uniform_unsigned(1001, 9999));
		break;
	}

	std::string target_file(target_dir + "/" + file_name + ".png");

	// Check existing 'tmp' directory
	if (!fs::is_directory("tmp") || !fs::exists("tmp"))
	{
		// If not existing -> create directory
		fs::create_directory("tmp");
	}

	// Этот код блять сплошная обработка ошибок, плюсом этому...
	// ... libpng - прекрасен, но блять, можете называть функции и переменные...
	// ... вкладывая в них хоть какой-то блять смысл
	// ЗАЕБЛО!!!!

	// Open file for writing by C FILE data structure
	FILE* file = fopen(target_file.data(), "wb");
	if (!file)
	{
		std::cerr << "gen_image ERROR: Cannot open file for writing\n";
		return 1;
	}

	// Create pointer to png structure of libpng
	png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr)
	{
		fclose(file);
		pixels.clear();
		std::cerr << "gen_image ERROR: Cannot create png_struct pointer entity\n";
		return 1;
	}

	// Create pointer to info section structure of libpng
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
		fclose(file);
		pixels.clear();
		std::cerr << "gen_image ERROR: Cannot create png_info struct pointer entity\n";
		return 1;
	}

	// Jmp - weird error checking method in libpng
	// I dont know what fuck is this. It was just in docs
	// It checks if a context has been created to write image data
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
		fclose(file);
		pixels.clear();
		std::cerr << "gen_image ERROR: Error occured creating libpng write context\n";
		return 1;
	}

	// Create io stream for libpng
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

	// Write pixels data
	png_byte row[sizeof(png_byte)*width*4];
	for (unsigned int line = 0; line < height; line++)
	{
		// Writing data line by line
		for (unsigned int x_line = 0; x_line < width; x_line++)
		{
			unsigned int index = x_line + line * height;
			Pixel* pixel = &pixels[index];
			// ...it must be unsigned char, okay
			*(row + x_line * 4 + 0) = static_cast<unsigned char>(pixel->r); // Red
			*(row + x_line * 4 + 1) = static_cast<unsigned char>(pixel->g); // Green
			*(row + x_line * 4 + 2) = static_cast<unsigned char>(pixel->b); // Blue
			*(row + x_line * 4 + 3) = static_cast<unsigned char>(pixel->a); // Alpha
			
		}
		png_write_row(png_ptr, row);
	}

	// Close libpng io stream
	png_write_end(png_ptr, NULL);

	// Release resources
	png_destroy_write_struct(&png_ptr, &info_ptr);
	fclose(file);
	pixels.clear();

	std::cout <<  "File: " << target_file << "\n";

	// Exit code 0 - success
	return 0;
}