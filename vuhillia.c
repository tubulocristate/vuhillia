#ifndef UNIQUE_FOR_THIS_FILE_
#define UNIQUE_FOR_THIS_FILE_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

typedef struct Matrix {
	size_t rows;
	size_t cols;
	float *data;
} Matrix;

typedef struct Vector3D{
	float x;
	float y;
	float z;
} Vector3D;


void swap(int *number1, int *number2);
void bresenham_line(uint32_t *pixels, size_t width, size_t height, int x0, int y0, int x1, int y1, uint32_t color);
void bresenham_centered_line(uint32_t *pixels, size_t width, size_t height, int x0, int y0, int x1, int y1, uint32_t color);
uint32_t encode_color(uint8_t alpha, uint8_t red, uint8_t green, uint8_t blue);
void fill_everething(uint32_t *pixels, size_t width, size_t height, uint32_t color);
int save_to_ppm_file(uint32_t *pixels, size_t width, size_t height, const char *file_path);
int min(int a, int b);
int max(int a, int b);
int cross2D(int x0, int y0, int x1, int y1);
void draw_triangle(uint32_t *pixels, size_t width, size_t height, int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
void draw_centered_triangle(uint32_t *pixels, size_t width, size_t height, int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
void draw_interpolated_triangle(uint32_t *pixels, size_t width, size_t height, int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color0, uint32_t color1, uint32_t color2);
void draw_interpolated_centered_triangle(uint32_t *pixels, size_t width, size_t height, int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color0, uint32_t color1, uint32_t color2);
void draw_filled_circle(uint32_t *pixels, size_t width, size_t height, int center_x, int center_y, float radius, uint32_t color);
Matrix matrix_allocate(size_t rows, size_t cols);
void matrix_print(Matrix matrix);
void matrix_fill(Matrix matrix, float value);
void matrix_array_fill(Matrix matrix, float *array);
void matrix_multiply(Matrix matrix_result, Matrix matrix_left, Matrix matrix_right);
void matrix_to_homogeneous(Matrix matrix_homogeneous, Matrix matrix);
Vector3D cross3D(Vector3D A, Vector3D B);
float euclidean_norm(Vector3D vector);
Vector3D normalize(Vector3D vector);
Vector3D substract_vectors(Vector3D A, Vector3D B);
void matrix_identity(Matrix matrix);
void World2CameraMatrix(Matrix matrix, Vector3D camera_position, Vector3D camera_looking_at, Vector3D up);
void matrix_transpose(Matrix transposed, Matrix matrix);



#endif


#ifdef IMPLEMENTATION_VUHILLIA

void swap(int *number1, int *number2)
{
	int dummy = *number1;
	*number1 = *number2;
	*number2 = dummy;
}

void bresenham_line(uint32_t *pixels, size_t width, size_t height, int x0, int y0, int x1, int y1, uint32_t color)
{
	size_t total_number_of_pixels = width*height;
	int is_steep = abs(y1 - y0) > abs(x1 - x0);
	if (is_steep) {
		swap(&x0, &y0);
		swap(&x1, &y1);
	}
	if (x1 < x0) {
		swap(&x0, &x1);
		swap(&y0, &y1);
	}
	
	float slope;
	if ((x1 - x0) == 0) {
		slope = 1;
	}
	else {
		slope = (float)(y1 - y0) / (x1 - x0);
	}
	
	float Y = y0;

	if (is_steep){
		for (int x = x0; x <= x1; x++){
			int y = (int)Y;
			if ((x*width + y) < total_number_of_pixels){
				pixels[x*width + y] = color;
				Y += slope;
			}else break;
		}
	}
	else {
		for (int x = x0; x <= x1; x++){
			int y = (int)Y;
			if ((y*width + x) < total_number_of_pixels){
				pixels[y*width + x] = color;
				Y += slope;
			}else break;
		}
	}

}

void bresenham_centered_line(uint32_t *pixels, size_t width, size_t height, int x0, int y0, int x1, int y1, uint32_t color)
{
	bresenham_line(pixels, width, height, x0+width/2, y0+height/2, x1+width/2, -y1+height/2, color);
}

uint32_t encode_color(uint8_t alpha, uint8_t red, uint8_t green, uint8_t blue)
{
	return alpha<<(8*3) | red<<(8*2) | green<<(8*1) | blue<<(8*0);
}

void fill_everething(uint32_t *pixels, size_t width, size_t height, uint32_t color)
{
	for (size_t y  = 0; y < height; y++){
		for (size_t x = 0; x < width; x++){
			pixels[y*width + x] = color;
		}
	}
}

int save_to_ppm_file(uint32_t *pixels, size_t width, size_t height, const char *file_path)
{
	FILE *f = NULL;
	f = fopen(file_path, "wb");

	if (f == NULL) {
		return -1;
	}
	fprintf(f, "P6\n%zu %zu 255\n", width, height);
	if (ferror(f)) {
		fclose(f);
		return -1;
	}

	for (size_t i = 0; i < width * height; i++){
		uint32_t pixel = pixels[i];
		uint8_t bytes[3] = {
			(pixel>>(8*2))&0xFF,
			(pixel>>(8*1))&0xFF,
			(pixel>>(8*0))&0xFF,
		};
		fwrite(bytes, sizeof(bytes), 1, f);
		if (ferror(f)) {
			fclose(f);
			return -1;
		}
	}
	return 0;
}

int min(int a, int b)
{
	if (a < b) return a;
	else return b;
}

int max(int a, int b)
{
	if (a > b) return a;
	else return b;
}

int cross2D(int x0, int y0, int x1, int y1)
{
	return	x0*y1 - y0*x1; 
}

void draw_triangle(uint32_t *pixels, size_t width, size_t height, int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
	size_t total_number_of_pixels = width*height;

	int v0v1x = x1 - x0;
	int v0v1y = y1 - y0;
	int v1v2x = x2 - x1;
	int v1v2y = y2 - y1;
	int v2v0x = x0 - x2;
	int v2v0y = y0 - y2;

	int MIN_X = min(min(x0, x1), x2);
	int MAX_X = max(max(x0, x1), x2);
	int MIN_Y = min(min(y0, y1), y2);
	int MAX_Y = max(max(y0, y1), y2);

	for (int y = MIN_Y; y <= MAX_Y; y++) {
		for (int x = MIN_X; x <= MAX_X; x++) {
			int alpha = cross2D(x-x0, y-y0, v0v1x, v0v1y);
		      	int beta  = cross2D(x-x1, y-y1, v1v2x, v1v2y);
		      	int gama  = cross2D(x-x2, y-y2, v2v0x, v2v0y);
			int is_inside = ( ((alpha >= 0)&&(beta >= 0)&&(gama >= 0)) || ((alpha <= 0)&&(beta <= 0)&&(gama <= 0)) );
			if ( is_inside ) {
				if ((y*width + x) < total_number_of_pixels) {
					pixels[y*width + x] = color;
				}
			}
		}
	}
}


void draw_centered_triangle(uint32_t *pixels, size_t width, size_t height, int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
	draw_triangle(pixels, width, height, x0+width/2, -y0+height/2, x1+width/2, -y1+height/2, x2+width/2, -y2+height/2, color);
}

void draw_interpolated_triangle(uint32_t *pixels, size_t width, size_t height, int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color0, uint32_t color1, uint32_t color2)
{
	size_t total_number_of_pixels = width*height;

	int v0v1x = x1 - x0;
	int v0v1y = y1 - y0;
	int v1v2x = x2 - x1;
	int v1v2y = y2 - y1;
	int v2v0x = x0 - x2;
	int v2v0y = y0 - y2;

	int MIN_X = min(min(x0, x1), x2);
	int MAX_X = max(max(x0, x1), x2);
	int MIN_Y = min(min(y0, y1), y2);
	int MAX_Y = max(max(y0, y1), y2);

	float area = cross2D(x2-x0, y2-y0, x1-x0, y1-y0);

	for (int y = MIN_Y; y <= MAX_Y; y++) {
		for (int x = MIN_X; x <= MAX_X; x++) {
			float alpha = cross2D(x-x0, y-y0, v0v1x, v0v1y) / area;
		      	float beta  = cross2D(x-x1, y-y1, v1v2x, v1v2y) / area;
		      	float gama  = cross2D(x-x2, y-y2, v2v0x, v2v0y) / area;
			int is_inside = ( ((alpha >= 0)&&(beta >= 0)&&(gama >= 0)) || ((alpha <= 0)&&(beta <= 0)&&(gama <= 0)) );
			if (is_inside) {
				if ((y*width + x) < total_number_of_pixels) {
					uint8_t red =   beta  *((color0>>(8*2))&0xFF) +
						        gama  *((color1>>(8*2))&0xFF) + 
						        alpha *((color2>>(8*2))&0xFF);
					uint8_t green = beta  *((color0>>(8*1))&0xFF) +
						        gama  *((color1>>(8*1))&0xFF) + 
						        alpha *((color2>>(8*1))&0xFF);
					uint8_t blue =  beta  *((color0>>(8*0))&0xFF) +
						        gama  *((color1>>(8*0))&0xFF) + 
						        alpha *((color2>>(8*0))&0xFF);
					uint32_t color = encode_color(255, red, green, blue);
					pixels[y*width + x] = color;
				}
			}
		}
	}
}


void draw_interpolated_centered_triangle(uint32_t *pixels, size_t width, size_t height, int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color0, uint32_t color1, uint32_t color2)
{
	draw_interpolated_triangle(pixels, width, height, x0+width/2, -y0+height/2, x1+width/2, -y1+height/2, x2+width/2, -y2+height/2, color0, color1, color2);
}

void draw_filled_circle(uint32_t *pixels, size_t width, size_t height, int center_x, int center_y, float radius, uint32_t color)
{
	float radius_squared = radius * radius;
	size_t total_number_of_pixels = width*height;

	for (int x = -radius; x <= radius; x++) {
		for (int y = -radius; y <= radius; y++) {
			if ( ((x*x + y*y) <= radius_squared) && ((x+center_x)*width + (y+center_y)) < total_number_of_pixels ) {
				pixels[(x+center_x)*width + (y+center_y)] = color;
			}
		}
	}
}

Matrix matrix_allocate(size_t rows, size_t cols)
{
	Matrix matrix;
	matrix.rows = rows;
	matrix.cols = cols;
	matrix.data = malloc(sizeof(*matrix.data)*rows*cols);
	return matrix;
}

void matrix_print(Matrix matrix)
{
	for (size_t row = 0; row < matrix.rows; row++) {
		for (size_t col = 0; col < matrix.cols; col++) {
			printf("%.2f\t", matrix.data[row*matrix.cols + col]);
		}
		printf("\n");
	}
}

void matrix_fill(Matrix matrix, float value)
{
	for (size_t row = 0; row < matrix.rows; row++) {
		for (size_t col = 0; col < matrix.cols; col++) {
			matrix.data[row*matrix.cols + col] = value;
		}
	}
}

void matrix_array_fill(Matrix matrix, float *array)
{
	for (size_t row = 0; row < matrix.rows; row++) {
		for (size_t col = 0; col < matrix.cols; col++) {
			matrix.data[row*matrix.cols + col] = array[row*matrix.cols + col];
		}
	}
}

void matrix_multiply(Matrix matrix_result, Matrix matrix_left, Matrix matrix_right)
{
	assert (matrix_left.cols == matrix_right.rows);
	assert (matrix_result.rows == matrix_left.rows);
	assert (matrix_result.cols == matrix_right.cols);
	for (size_t row = 0; row < matrix_left.rows; row++) {
		for (size_t col = 0; col < matrix_right.cols; col++) {
			float value = 0;
			for (size_t k = 0; k < matrix_left.cols; k++) {
				value = value + matrix_left.data[row*matrix_left.cols + k] * 
					        matrix_right.data[k*matrix_right.cols + col];
			}
			matrix_result.data[row*matrix_result.cols + col] = value;
		}
	}
}

void matrix_to_homogeneous(Matrix matrix_homogeneous, Matrix matrix)
{
	matrix_fill(matrix_homogeneous, 1);
	for (size_t row = 0; row < matrix.rows; row++) {
		for (size_t col = 0; col < matrix.cols; col++) {
			matrix_homogeneous.data[row*matrix_homogeneous.cols + col] = 
				    matrix.data[row*matrix.cols + col];
		}
	}
}

Vector3D cross3D(Vector3D A, Vector3D B)
{
	Vector3D perpendicular = {A.y*B.z - A.z*B.y, A.z*B.x - A.x*B.z, A.x*B.y - A.y*B.x};
	return	perpendicular; 
}

float euclidean_norm(Vector3D vector)
{
	return sqrt(vector.x*vector.x+vector.y*vector.y+vector.z*vector.z);
}

Vector3D normalize(Vector3D vector)
{
	float norm = euclidean_norm(vector);
	Vector3D normalized = {vector.x / norm, vector.y / norm, vector.z / norm};
	return normalized;
}

Vector3D substract_vectors(Vector3D A, Vector3D B)
{
	Vector3D C = {A.x - B.x, A.y - B.y, A.z - B.z};
	return C;
}

void matrix_identity(Matrix matrix)
{
	matrix_fill(matrix, 0);
	for (size_t i = 0; i < matrix.rows; i++) {
		matrix.data[i*matrix.rows + i] = 1;
	}
}

void World2CameraMatrix(Matrix matrix, Vector3D camera_position, Vector3D camera_looking_at, Vector3D up)
{
	Vector3D camera_direction = normalize(substract_vectors(camera_position, camera_looking_at));
	Vector3D camera_right = normalize(cross3D(up, camera_direction));
	Vector3D camera_up = cross3D(camera_direction, camera_right);

	matrix_identity(matrix);
	matrix.data[0*4 + 0] = camera_right.x;
	matrix.data[0*4 + 1] = camera_right.y;
	matrix.data[0*4 + 2] = camera_right.z;

	matrix.data[1*4 + 0] = camera_up.x;
	matrix.data[1*4 + 1] = camera_up.y;
	matrix.data[1*4 + 2] = camera_up.z;

	matrix.data[2*4 + 0] = camera_direction.x;
	matrix.data[2*4 + 1] = camera_direction.y;
	matrix.data[2*4 + 2] = camera_direction.z;

	matrix.data[3*4 + 0] = -camera_position.x;
	matrix.data[3*4 + 1] = -camera_position.y;
	matrix.data[3*4 + 2] = -camera_position.z;
}

void matrix_transpose(Matrix transposed, Matrix matrix)
{
	for (size_t row = 0; row < matrix.rows; row++) {
		for (size_t col = 0; col < matrix.cols; col++) {
			transposed.data[col*transposed.cols + row] = matrix.data[row*matrix.cols + col];
		}
	}
}




#endif 

