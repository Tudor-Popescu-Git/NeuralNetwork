/*
 * Layer.cpp

 *
 *  Created on: Oct 24, 2016
 *      Author: root
 */
#include "stdafx.h"
#include "../_h/Layer.h"
#include "../_h/Primitives.h"
#include "../_h/rapidxml.hpp"

using namespace std;
using namespace rapidxml;

Layer::Layer(unsigned int num, Types type, LayerConnector &connector, double &learningFactor, double &momentumFactor, int NUMBER_OF_SLICES) :
	type(type), connector(connector), learningFactor(learningFactor), momentumFactor(momentumFactor), NUMBER_OF_SLICES(NUMBER_OF_SLICES){
	Neuron *nptr = nullptr;

	neurons.reserve(num);
	from.reserve(num);
	to.reserve(num);
	inputs.reserve(num);
	biases.reserve(num);
	outputs.reserve(num);
	errors.reserve(num);
	deltas.reserve(num);
	slices.reserve(NUMBER_OF_SLICES);
	int i;
	for (i = 0; i < num; i++) {
		nptr = new Neuron(type);
		neurons.push_back(nptr);

		from.push_back(&(*nptr).from);
		to.push_back(&(*nptr).to);

		inputs.push_back(&nptr->input);
		biases.push_back(&nptr->bias);
		outputs.push_back(&nptr->output);
		deltas.push_back(&nptr->delta);
		errors.push_back(&nptr->error);
		
	}
	auto neuronBegin = neurons.begin();
	auto slicesDivisor = [&]() {
		int a = num / NUMBER_OF_SLICES;
		int b = num % NUMBER_OF_SLICES;
		int currentIndex = 0;
		for (int i = 0; i < NUMBER_OF_SLICES; i++) {
			if (b > 0) {
				slices.emplace_back(*this, neuronBegin + currentIndex, neuronBegin + currentIndex + a + 1);
				currentIndex += a + 1;
				b--;
			}
			else {
				slices.emplace_back(*this, neuronBegin + currentIndex, neuronBegin + currentIndex + a);
				currentIndex += a;
			}
		}
	};
	slicesDivisor();
	/*
	const size_t SLICE_SIZE = num / NUMBER_OF_SLICES;
	if (SLICE_SIZE > 0) {
		for (i = 0; i < NUMBER_OF_SLICES - 1; i++) {
			slices.emplace_back(*this, neuronBegin + i * SLICE_SIZE, neuronBegin + (i + 1) * SLICE_SIZE - 1);
		}
	}
	
	slices.emplace_back(*this, neuronBegin + i * SLICE_SIZE, neurons.end());
	*/
}
Layer::Layer(LayerConnector &connector, double &learningFactor, double &momentumFactor, rapidxml::xml_document<> &doc, rapidxml::xml_node<> &parent) : connector(connector), learningFactor(learningFactor),momentumFactor(momentumFactor) {
	loadXML(doc, parent);
}
RET Layer::slicesDivisor(int numSlices) {
	return RET::OK;
}
Layer::iterator Layer::begin() {
return this->neurons.begin();
}
Layer::iterator Layer::end() {
	return this->neurons.end();
}
Layer& Layer::bind(Layer& nextLayer) {
	connector.connect(*this, nextLayer);

	this->nextLayer = &nextLayer;
	return *this;

}
int Layer::activate(void) {
	/*
	if (type == Types::IN) {
		for (auto& neuronPtr : neurons) {
			neuronPtr->output = neuronPtr->input;
		}
		return 0;
	}
	for (auto& neuronPtr : neurons) {
		neuronPtr->input = neuronPtr->bias;
		for (auto& syn : neuronPtr->from) {
			neuronPtr->input += syn->from->output * syn->weight;
		}
	}
	NeuronNetwork::activationV(inputs, outputs);
	*/
	for (auto sliceIt = slices.begin(); sliceIt != slices.end(); sliceIt++) {
		activate_S(*sliceIt);
	}

	return 0;
}
RET Layer::activate_S(LayerSlice<iterator> &slice) {
	Neuron *neuronPtr = nullptr;
	if (type == Types::IN) {
		for (auto neuronIt = slice.begin(); neuronIt != slice.end(); neuronIt++) {
			neuronPtr = (*neuronIt);
			neuronPtr->output = neuronPtr->input;
		}
	}
	else {
		for (auto neuronIt = slice.begin(); neuronIt != slice.end(); neuronIt++) {
			neuronPtr = (*neuronIt);
			neuronPtr->input = neuronPtr->bias;
			for (auto& syn : neuronPtr->from) {
				neuronPtr->input += syn->from->output * syn->weight;
			}
			NeuronNetwork::activation(neuronPtr->input, neuronPtr->output);
		}
	}
	return RET::OK;
}
int Layer::backprop(void) {
	//deltaPass();
	//updatePass();
	for (auto &slice : slices) {
		deltaPass_S(slice);
	}
	for (auto &slice : slices) {
		updatePass_S(slice);
	}
	return 0;
}
int Layer::deltaPass(void) {
#ifdef SIMBRAIN_COMPAT
	auto f = [this]()
	{ auto neurons_it = begin();
	for (; neurons_it != end(); neurons_it++) {
		(*neurons_it)->input -= (*neurons_it)->bias;
	}
	};
#else
	auto f = []() {};
#endif
	switch (type) {

	case(Types::OUT): {
		NeuronNetwork::costDV(NeuronNetwork::expected, outputs, errors);
		f();
		NeuronNetwork::activationDV(inputs, inputs);
		hadamardDot(errors, inputs, deltas);
	}break;

	case(Types::HIDDEN):
	{
		f();
		NeuronNetwork::activationDV(inputs, inputs);
		auto neurons_it = neurons.begin();
		for (; neurons_it != neurons.end(); neurons_it++) {
			auto &delta = (*neurons_it)->delta;
			delta = 0;
			for (auto& to_it = (*neurons_it)->to.begin(); to_it != (*neurons_it)->to.end(); to_it++) {
				delta += (*to_it)->weight * (*to_it)->to->delta;
			}
		}
		hadamardDot(deltas, inputs, deltas);
	}break;

	}
	return 0;
}
RET Layer::deltaPass_S(LayerSlice<iterator> &slice) {
#ifdef SIMBRAIN_COMPAT
	auto f = [this](LayerSlice<iterator> &slice)
	{ auto neurons_it = slice.begin();
	for (; neurons_it != slice.end(); neurons_it++) {
		(*neurons_it)->input -= (*neurons_it)->bias;
	}
	};
#else
	auto f = [](LayerSlice<iterator> &slice) {};
#endif
	switch (type) {

	case(Types::OUT): {
		//NeuronNetwork::costDV(NeuronNetwork::expected, outputs, errors);
		cost_DS(NeuronNetwork::expected, slice);
		f(slice);
		//NeuronNetwork::activationDV(inputs, inputs);
		activation_DS(slice);
		//hadamardDot(errors, inputs, deltas);
		hadamardDot_S(slice);
	}break;

	case(Types::HIDDEN):
	{
		f(slice);

		//NeuronNetwork::activationDV(inputs, inputs);
		activation_DS(slice);
		auto neurons_it = slice.begin();
		for (; neurons_it != slice.end(); neurons_it++) {
			auto &delta = (*neurons_it)->delta;
			delta = 0;
			for (auto& to_it = (*neurons_it)->to.begin(); to_it != (*neurons_it)->to.end(); to_it++) {
				delta += (*to_it)->weight * (*to_it)->to->delta;
			}
		}
		//hadamardDot(deltas, inputs, deltas);
		hadamardDot_S(slice);
	}break;

	}
	return RET::OK;
}
RET Layer::cost_DS(vector<double> &expected, LayerSlice<iterator> &slice) {
	auto expectedIt = expected.begin();
	auto sliceIt = slice.begin();
	for (; expectedIt != expected.end(); expectedIt++, sliceIt++){
		//NeuronNetwork::cost_D(*expectedIt, **outputIt, **errorsIt);
		NeuronNetwork::cost_D((*expectedIt + slice.startIndex), (*sliceIt)->output, (*sliceIt)->error);
	}
	return RET::OK;
}
RET Layer::activation_DS(LayerSlice<iterator> &slice) {
	auto neuronIt = slice.begin();
	for (; neuronIt != slice.end(); neuronIt++) {
		NeuronNetwork::activation_D((*neuronIt)->input, (*neuronIt)->input);
	}
	return RET::OK;
}
RET Layer::hadamardDot_S(LayerSlice<iterator> &slice) {
	auto sliceIt = slice.begin();
	for (; sliceIt != slice.end(); sliceIt++) {
		(*sliceIt)->delta = (*sliceIt)->input * (*sliceIt)->delta;
	}
	return RET::OK;
}
int Layer::updatePass(void) {
	auto f = [this]()
	{
		auto neurons_it = neurons.begin();
		Synapse *synapse;
		double currentDelta;
		for (; neurons_it != neurons.end(); neurons_it++) {

			for (auto& from_it = (*neurons_it)->from.begin(); from_it != (*neurons_it)->from.end(); from_it++) {
				synapse = (*from_it);
				currentDelta = learningFactor * (*neurons_it)->delta * synapse->from->output;
				synapse->weight -= currentDelta + momentumFactor * synapse->previousDelta;
				synapse->previousDelta = currentDelta;

			}
			(*neurons_it)->bias -= learningFactor * (*neurons_it)->delta;
		}
	};
	switch (type) {

		case(Types::OUT):
		{
			f();
		}break;

		case(Types::HIDDEN):
		{
			f();
		}break;

	}
	return 0;
}
RET Layer::updatePass_S(LayerSlice<iterator> &slice) {
	auto f = [this](LayerSlice<iterator> &slice)
	{
		auto neurons_it = slice.begin();
		Synapse *synapse;
		double currentDelta;
		for (; neurons_it != slice.end(); neurons_it++) {

			for (auto& from_it = (*neurons_it)->from.begin(); from_it != (*neurons_it)->from.end(); from_it++) {
				synapse = (*from_it);
				currentDelta = learningFactor * (*neurons_it)->delta * synapse->from->output;
				synapse->weight -= currentDelta + momentumFactor * synapse->previousDelta;
				synapse->previousDelta = currentDelta;

			}
			(*neurons_it)->bias -= learningFactor * (*neurons_it)->delta;
		}
	};
	switch (type) {

	case(Types::OUT):
	{
		f(slice);
	}break;

	case(Types::HIDDEN):
	{
		f(slice);
	}break;

	}
	return RET::OK;
}
size_t Layer::size(void) {
	return neurons.size();
}
RET Layer::saveXML(rapidxml::xml_document<> &doc, rapidxml::xml_node<> &parent) {

	auto node = doc.allocate_node(rapidxml::node_type::node_element, "Layer");
	parent.append_node(node);
	for (auto &neuron : neurons) {
		neuron->saveXML(doc, *node);
	}
	return RET::OK;
}
RET Layer::loadXML(rapidxml::xml_document<> &doc, rapidxml::xml_node<> &parent) {
	if (strcmp(parent.name(), "Layer")) {
		return RET::NOK;
	}
	auto neuronNode = parent.first_node("Neuron");
	for (; neuronNode && !strcmp(neuronNode->name(), "Neuron"); neuronNode = neuronNode->next_sibling()) {
		neurons.push_back(new Neuron(doc, *neuronNode));
	}
	return RET::OK;
}
