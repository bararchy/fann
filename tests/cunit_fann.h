#ifndef _CUNIT_FANN_
#define _CUNIT_FANN_

#define IS_NEG(x)  ((x) < -0.95)
#define IS_POS(x)  ((x) > 0.95)

void cunit_xor_train(void);
void cunit_xor_test(void);
void cunit_steepness_train(void);
void cunit_simple_train(void);
void cunit_simple_test(void);

#endif
