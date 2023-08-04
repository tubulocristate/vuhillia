#define IMPLEMENTATION_VUHILLIA
#include "vuhillia.c"

#define WIDTH 800
#define HEIGHT 600
#define BACKGROUND 0x000000

static uint32_t pixels[WIDTH*HEIGHT];


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


void draw_figure(pixels, width, height, points, faces)
{
	for (size_t i = 0; i < *faces.rows; i++) {
		size_t v0 = faces.data[i*(*faces.cols) + 0];
		size_t v1 = faces.data[i*(*faces.cols) + 1];
		size_t v2 = faces.data[i*(*faces.cols) + 2];
		Vector3D n0 = get_vector_by_row(points, v0);
		Vector3D n1 = get_vector_by_row(points, v1);
		Vector3D n2 = get_vector_by_row(points, v2);
		draw_centered_triangle(pixels, WIDTH, HEIGHT, 
				       n0.x*700,
				       n0.y*700,
				       n1.x*700,
				       n1.y*700,
				       n2.x*700,
				       n2.y*700,
				       0xFFFFFFFF);
	}

}
					     

//void rotate_around_x(Matrix rotated, Matrix matrix, float angle)
//{
	//float cosinus = cos(angle);
	//float sinus = sin(angle);
	//Matrix rotation_matrix = matrix_allocate(3, 3);
	//matrix_fill(rotation_matrix, 0);
	//rotation_matrix.data[0*3 + 0] = 1;
	//rotation_matrix.data[1*3 + 1] = cosinus;
	//rotation_matrix.data[1*3 + 2] = -sinus;
	//rotation_matrix.data[2*3 + 1] = sinus;
	//rotation_matrix.data[2*3 + 2] = cosinus;
	//matrix_multiply(rotated, rotation_matrix, matrix);
	//matrix_dealocate(rotation_matrix);
//
//}
//
//
int main(void)
{
	Vector3D camera_position = {1, 2, 3};
	Vector3D camera_looking_at = {0, 0, 0};
	Vector3D up = {0, 1, 0};
	
	float fov = 90;
	float z_near = 0.01;
	float z_far = 10;

	Matrix Cube = matrix_allocate(8, 3); 
	Matrix Faces = matrix_allocate(12, 3);
	matrix_array_fill(Faces, faces);
	matrix_array_fill(Cube, cube);
	center_figure(Cube);

	Matrix points = matrix_allocate(4, 8);
	project_points(points, Cube, camera_position, camera_looking_at, up, fov, z_near, z_far);


	const char *file_path = "cube.ppm";
	save_to_ppm_file(pixels, WIDTH, HEIGHT, file_path);
	
	matrix_dealocate(Cube);
	matrix_dealocate(points);
	matrix_dealocate(Faces);
	return 0;
}
