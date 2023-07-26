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
	draw_interpolated_triangle(pixels, WIDTH, HEIGHT, 0, 0, 50, 200, -400, 300, color0, color1, color2);
	save_to_ppm_file(pixels, WIDTH, HEIGHT, file_path);

	Matrix matrix_left = matrix_allocate(3, 3);
	float array[] = {1., 2., 3., 
			  4., 5., 6.,
	   		  7., 8., 9.};
	matrix_array_fill(matrix_left, array);
	matrix_print(matrix_left);	
	
	return 0;
}
