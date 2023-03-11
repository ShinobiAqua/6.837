#include<iostream>
#include"image.h"
#include"matrix.h"
#include"vectors.h"
#include"ifs.h"
using namespace std;
char input_file[100];
char output_file[100];
int num_points = 10000;
int num_iters = 10;
int sz = 100;

int main() {
	printf("input filename: ");
	scanf("%s", input_file);
	printf("points number:  ");
	scanf("%d", &num_points);
	printf("num of iterations: ");
	scanf("%d", &num_iters);
	printf("size: ");
	scanf("%d", &sz);
	printf("output filename: ");
	scanf("%s", output_file);
	Image img(sz, sz);
	IFS ifs;
	//cout << input_file;
	ifs.read(input_file);
	ifs.render(img, num_points, num_iters);
	img.SaveTGA(output_file);
	return 0;
}