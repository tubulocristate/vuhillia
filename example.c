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
	//Vector3D camera_position = {0, 0, -3};
	//Vector3D camera_looking_at = {0, 0, 0};
	//Vector3D up = {0, 1, 0};
	//float fov = M_PI;
	//float z_near = 0.1;
	//float z_far = 10;

	//Matrix CameraM = matrix_allocate(4, 4);
	//CameraMatrix(CameraM, camera_position, camera_looking_at, up, fov, z_near, z_far);
	//matrix_print(CameraM);
	//matrix_dealocate(CameraM);
	Matrix Cube = matrix_allocate(8, 3);
	matrix_array_fill(Cube, cube);
	matrix_print(Cube);
	printf("___________\n");
	center_figure(Cube);
	matrix_print(Cube);
	return 0;
}
