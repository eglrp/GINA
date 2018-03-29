

//#include "main.h"

#include "TutorialConfig.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "linalg.h"
#include "alglibinternal.h"
#include "GPSWeekSecond.hpp"
#include "BasicFramework.hpp"
#include "PRSolution.hpp"


#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

int main(int argc, char **argv) {

	Matrix2d a;
	a << 1, 2,
		3, 4;
	MatrixXd b(2, 2);
	b << 2, 3,
		1, 4;
	std::cout << "a + b =\n" << a + b << std::endl;
	std::cout << "a - b =\n" << a - b << std::endl;
	std::cout << "Doing a += b;" << std::endl;
	a += b;
	std::cout << "Now a =\n" << a << std::endl;
	Vector3d v(1, 2, 3);
	Vector3d w(1, 0, 0);
	std::cout << "-v + w - v =\n" << -v + w - v << std::endl;


	alglib::boolean_1d_array b1;
	b1 = "[true]";

	gpstk::PRSolution solution;

	return 0;
}