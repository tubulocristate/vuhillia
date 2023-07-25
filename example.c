#define IMPLEMENTATION_VUHILLIA
#include "vuhillia.c"

#define WIDTH 800
#define HEIGHT 600
#define BACKGROUND 0x000000

static uint32_t pixels[WIDTH*HEIGHT];

int main(void)
{
	uint32_t color0 = encode_color(255, 255, 0,   0);
	uint32_t color1 = encode_color(255, 0,   255, 0);
	uint32_t color2 = encode_color(255, 0,   0,   255);
	const char *file_path = "triangle.ppm";
	fill_everething(pixels, WIDTH, HEIGHT, BACKGROUND);
	draw_interpolated_centered_triangle(pixels, WIDTH, HEIGHT, 220, 0, 0, -200, -400, 300, color0, color1, color2);
	save_to_ppm_file(pixels, WIDTH, HEIGHT, file_path);

	return 0;
}
