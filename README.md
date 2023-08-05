# vuhillia
**Inspired by [Tsoding](https://github.com/tsoding)**.<br >Simple header-only graphics library. Still in development. Line, circle, and triangle (with color interpolation between vertices) drawing is available. Rendering of 3D objects is to be implemented. 

## Gradient Triangle
```c
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
```

### result
![alt text](https://github.com/tubulocristate/vuhillia/blob/main/triangle.jpg?raw=true)

## Cube with depth
```c
#define IMPLEMENTATION_VUHILLIA
#include "vuhillia.c"

#define WIDTH 800
#define HEIGHT 600
#define BACKGROUND 0x000000

static uint32_t pixels[WIDTH*HEIGHT];
static float z_buffer[WIDTH*HEIGHT];

float cube[] = {0, 0, 0,
		1, 0, 0,
		1, 1, 0,
		0, 1, 0,
		0, 0, 1,
		1, 0, 1,
		1, 1, 1,
		0, 1, 1};

float faces[] = {0, 1, 2,
		 0, 2, 3,
		 0, 4, 7,
		 0, 7, 3,
		 4, 0, 5,
		 1, 5, 0,
		 1, 5, 2,
		 2, 5, 6,
		 2, 6, 3,
		 3, 7, 6,
		 4, 5, 6,
		 4, 6, 7};


int main(void)
{
	for (size_t i = 0; i < WIDTH*HEIGHT; i++) {
		z_buffer[i] = -100;
	}
	Vector3D camera_position = {1, 2, 5};
	Vector3D camera_looking_at = {0, 0, 0};
	Vector3D up = {0, 1, 0};
	
	float fov = 90;
	float z_near = 0.1;
	float z_far = 100;

	Matrix Cube = matrix_allocate(8, 3); 
	Matrix Faces = matrix_allocate(12, 3);
	matrix_array_fill(Faces, faces);
	matrix_array_fill(Cube, cube);
	center_figure(Cube);

	Matrix points = matrix_allocate(4, 8);
	project_points(points, Cube, camera_position, camera_looking_at, up, fov, z_near, z_far);
	scale_figure(points, WIDTH, HEIGHT);	
	scale_z(points);
	
	draw_figure(pixels, z_buffer, WIDTH, HEIGHT, points, Faces);

	const char *file_path = "cube.ppm";
	save_to_ppm_file(pixels, WIDTH, HEIGHT, file_path);
	
	matrix_dealocate(Cube);
	matrix_dealocate(points);
	matrix_dealocate(Faces);
	return 0;
}
```
### result
![alt text](https://github.com/tubulocristate/vuhillia/blob/main/cube.jpg?raw=true)

