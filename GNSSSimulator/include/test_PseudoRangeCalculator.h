#pragma once
#include "PseudoRangeCalculator.h"
#include "PRSolution2.hpp"
#include "PRSolution.hpp"
#include "IonoModel.hpp"

int PseudoRangeCalculator_test1(void);
int PseudoRangeCalculator_test2(void);
int PseudoRangeCalculator_test3(void);
int PseudoRangeCalculator_test4(void);
int PseudoRangeCalculator_test5(void);

/* Test 6 Solution deviance ~4.5 cm in best cases. Error Iteration method is not finalized.
*/
int PseudoRangeCalculator_test6(void);

/* Proper iteration method, erorrs are -e09, base is test4. Test7 includes proper
	output logging to work with the Matlab
	evaluation scripts. Another addition is the initial TropModel Error
	and Iono corr.
*/
int PseudoRangeCalculator_test7(void);
