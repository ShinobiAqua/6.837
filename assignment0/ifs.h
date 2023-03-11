#include"matrix.h"
#include"image.h"
#include"vectors.h"
#include<stdlib.h>
#include<time.h>

class IFS
{
private:
	//a field to store n, the number of transformations
	int n;
	//an array of matrices representing the n transformations
	Matrix* matx;
	
	//an array of the corresponding probabilities for choosing a transformation
	float* prob;
	
public:
	//a constructor that creates an IFS
	IFS() {
		int n = 0;
		matx = NULL;
		prob = NULL;
	}
	//an input method that reads the IFS description
	void read(char* input_file);
	//a render method that takes as input an image instance, a number of pointsand a number of iterations
	void render(Image& img, int lots, int num_iters);
	//a destructor that frees the memory of the various arrays(using delete)
	~IFS()
	{
		delete[]matx;
		delete[]prob;
	}
};