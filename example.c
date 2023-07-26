#define IMPLEMENTATION_VUHILLIA
#include "vuhillia.c"

#define WIDTH 800
#define HEIGHT 600
#define BACKGROUND 0x000000

//static uint32_t pixels[WIDTH*HEIGHT];


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
		 0, 4, 5,
		 0, 5, 1,
		 1, 5, 2,
		 2, 5, 6,
		 2, 6, 3,
		 3, 7, 6,
		 4, 5, 6,
		 4, 6, 7};


int main(void)
{
	Vector3D camera_position = {0, 0, -3};
	Vector3D camera_looking_at = {0, 0, 0};
	Vector3D up = {0, 1, 0};
	Matrix W2CM = matrix_allocate(4, 4);
	World2CameraMatrix(W2CM, camera_position, camera_looking_at, up);
	matrix_print(W2CM);
	printf("_____________________\n");

	Matrix cube_points = matrix_allocate(8, 3);
	Matrix cube_homogeneous_points = matrix_allocate(8, 4);

	matrix_array_fill(cube_points, cube);
	matrix_to_homogeneous(cube_homogeneous_points, cube_points);
	matrix_print(cube_homogeneous_points);
	printf("_________________________\n");

	Matrix transformed_points = matrix_allocate(4, 8);
	Matrix cube_homogeneous_transposed = matrix_allocate(4, 8);
	matrix_transpose(cube_homogeneous_transposed, cube_homogeneous_points);
	matrix_multiply(transformed_points, W2CM, cube_homogeneous_transposed);
	matrix_transpose(cube_homogeneous_points, transformed_points);
	matrix_print(cube_homogeneous_points);
	
	return 0;
}
