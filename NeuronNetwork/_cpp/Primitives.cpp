
#include "stdafx.h"
#include "../_h/Primitives.h"

int sigmoid(double input, double bias, double &output) {
	double z = input + bias;
	return sigmoid(z, output);
}
int sigmoid(double z, double &output) {
	output = 1.00 / (1.00 + exp(-z));
	return 0;
}
/*
int sigmoidV(vector<double*>& input, vector<double*>& bias,
	vector<double*>& output) {
	vector<double*>::size_type sz = input.size();
	if ((sz != bias.size()) || sz != output.size()) {
		return 1;
	}

	for (auto inputIt = input.begin(), biasIt = bias.begin(), outputIt = output.begin();
		inputIt != input.end();
		inputIt++, biasIt++, outputIt++) {

		sigmoid(**inputIt, **biasIt, **outputIt);
	}

	return 0;
}
*/
int sigmoid_V(vector<double*>& input, vector<double*>& output) {
	vector<double*>::size_type sz = input.size();
	if (sz != output.size()) {
		return 1;
	}

	for (auto inputIt = input.begin(), outputIt = output.begin();
		inputIt != input.end();
		inputIt++, outputIt++) {

		sigmoid(**inputIt, **outputIt);
	}

	return 0;
}
int sigmoidDelta(double input, double bias, double &output) {
	double o, o2, z;
	z = input + bias;
	sigmoid(z, o);
	sigmoid(z, o2);
	output = o / o2;
	return 0;
}
int sigmoid_D(double z, double &output) {
	double temp;
	sigmoid(z, temp);
	output = temp * (1 - temp);
	return 0;
}

int sigmoidDeltaV(vector<double*>& input, vector<double*>& bias,
	vector<double*>& output) {
	auto inputIt = input.begin(), biasIt = bias.begin(), outputIt = output.begin();
	for (; inputIt != input.end(); inputIt++, biasIt++, outputIt++){
	sigmoid(**inputIt, **biasIt, **outputIt);
	}
	return 0;
}
int sigmoid_DV(vector<double*> &input, vector<double*> &output) {
	auto inputIt = input.begin(), outputIt = output.begin();
	double temp;
	sigmoid(**inputIt, temp);
	for (; inputIt != input.end(); inputIt++, outputIt++) {
		**outputIt = temp * (1 - temp);
	}
	return 0;
}
int quadratic(double expected, double output, double &error) {
	double temp;
	temp = expected - output;
	error = 0.5 * temp * temp;
	return 0;
}
int quadratic(vector<double*>expected, vector<double*> output, double& error) {
	error = 0;
	auto expected_it = expected.begin(), output_it = output.begin();
	double temp = 0;
	double sum = 0;
	for (; expected_it != expected.end(); expected_it++, output_it++) {
		quadratic(expected, output, error);
		sum += error;
		//temp = **expected_it - **output_it;
		//error += 0.5*temp*temp;
	}
	error = sum;
	return 0;
}
int quadratic_D(double expected, double output, double &error) {
	error = output - expected;
	return 0;
}
int quadratic_DV(vector<double> &expected, vector<double*> &output, vector<double*> &error) {
	auto expectedIt = expected.begin();
	auto outputIt = output.begin();
	auto errorIt = error.begin();
	double temp;
	for (; expectedIt != expected.end(); expectedIt++, outputIt++, errorIt++) {
		quadratic_D(*expectedIt, **outputIt, **errorIt);
		//**errorIt = (**outputIt) - (*expectedIt);
	}
	return 0;
}
int cost(vector<double*> &input, vector<double*> &output) {
	for (auto& outputIt = output.begin(), inputIt = input.begin(); outputIt != output.end(); outputIt++, inputIt++) {
		*outputIt = 0;

	}
	return 0;
}
int costError(vector<double*> &expected, vector<double*> &output, vector<double*> &error) {
	if (expected.size() != output.size() || error.size() != output.size()) {
		return -1;
	}
	auto expectedIt = expected.begin(), outputIt = output.begin(), errorIt = error.begin();
	for (; expectedIt != expected.end(); expectedIt++, outputIt++, errorIt++) {
		**errorIt = **expectedIt - **outputIt;
	}
	return 0;
}
int costDelta(vector<double*> inputX, vector<double*> &inputY, vector<double*> &output) {
	for (auto& outputIt = output.begin(), inputXIt = inputX.begin(), inputYIt = inputY.begin(); outputIt != output.end(); outputIt++, inputXIt++, inputYIt++) {
		**outputIt = **inputXIt - **inputYIt;
	}
	return 0;
}
int hadamardDot(vector<double*> &input1, vector<double*> &input2, vector<double*> &output) {
	if (input1.size() != input2.size() || input1.size() != output.size()) {
		return -1;
	}
	auto input1It = input1.begin(), input2It = input2.begin(), outputIt = output.begin();
	while (input1It != input1.end()) {
		(**outputIt) = (**input1It) * (**input2It);
		outputIt++; input1It++; input2It++;
	}
	return 0;
}
int addV(vector<double*> &input1, vector<double*> &input2, vector<double*> &output) {

	for (auto &input1It = input1.begin(), &input2It = input2.begin(), &outputIt = output.begin();
		input1It != input1.end();
		input1It ++, input2It++, outputIt++
		) 
	{
		(*(*outputIt)) = (*(*input1It)) + (*(*input2It));
	}
	return 0;
}
