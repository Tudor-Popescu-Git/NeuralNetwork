/*
 * Primitives.h
 *
 *  Created on: Nov 9, 2016
 *      Author: root
 */

#ifndef CPP_PRIMITIVES_H_
#define CPP_PRIMITIVES_H_

#include "../_h/Common.h"

int sigmoid(double input, double bias, double& output);
int sigmoid(double z, double &output);
//int sigmoidV(vector<double*>& input, vector<double*>& bias, vector<double*>& output);
int sigmoid_V(vector<double*>& input, vector<double*>& output);
int sigmoid_DV(vector<double*>& input,vector<double*> &output);
int sigmoid_D(double z, double &output);

int quadratic_D(double expected, double output, double &error);
int quadratic_DV(vector<double> &expected, vector<double*> &output, vector<double*> &error);
int costError(vector<double*> &expected, vector<double*> &output, vector<double*> &error);
int hadamardDot(vector<double*> &input1, vector<double*> &input2, vector<double*> &output);
int addV(vector<double*> &input1, vector<double*> &input2, vector<double*> &output);
#endif /* CPP_PRIMITIVES_H_ */
