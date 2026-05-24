#pragma once
#include <vector>
#include "../_h/LayerSlice.h"
#include "../_h/Primitives.h"

template<typename InputIt>
static RET sigmoid_S(const LayerSlice<InputIt> &input, const LayerSlice<InputIt> &output) {
	if (input.size() != output.size()) {
		return RET::ERR_SIZE;
	}
	for (auto inputIt = input.begin(), outputIt = output.begin();
		inputIt != input.end();
		inputIt++, outputIt++) {

		sigmoid(**inputIt, **outputIt);
	}
	return RET::OK;
}
template<typename InputIt>
static RET sigmoid_DS(const LayerSlice<InputIt> &inputSlice, const LayerSlice<InputIt> &outputSlice) {
	if (inputSlice.size() != outputSlice.size()) {
		return RET::ERR_SIZE;
	}
	auto inputIt = inputSlice.begin(), outputIt = outputSlice.begin();
	double temp;
	sigmoid_D(**inputIt, temp);
	for (; inputIt != inputSlice.end(); inputIt++, outputIt++) {
		sigmoid_D(**inputIt, **outputIt);
		//**outputIt = temp * (1 - temp);
	}
	return RET::OK;
}
template<typename InputIt>
static RET quadratic_DS(const LayerSlice<InputIt> &expectedSlice, const LayerSlice<InputIt> &outputSlice, const LayerSlice<InputIt> &errorSlice) {
	if (expectedSlice.size() != outputSlice.size() || outputSlice.size() != errorSlice.size()) {
		return RET::ERR_SIZE;
	}
	auto expectedIt = expectedSlice.begin(), outputIt = outputSlice.begin(), errorIt = errorSlice.begin();
	for (; expectedIt != expected.end(); expectedIt++, outputIt++, errorIt++) {
		quadratic_D(**expectedIt, **outputIt, **errorIt);
		//**errorIt = (**outputIt) - (**expectedIt);
	}
	return RET::OK;
}