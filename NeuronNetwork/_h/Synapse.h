#ifndef H_SYNAPSE_H_
#define H_SYNAPSE_H_

#include "../_h/Common.h"
#include "../_h/Neuron.h"
#include "../_h/rapidxml.hpp"
//class xml_document;
//class xml_node;
class Neuron;

struct Synapse {
	double weight;
	double previousDelta;

	Neuron *from = nullptr, *to = nullptr;
	
	Synapse(Neuron *from = nullptr, Neuron *to = nullptr, double weight = 0.5) :
			from(from), to(to), weight(weight), previousDelta(0.0) {
	}
	Synapse() = default;
	RET saveXML(rapidxml::xml_document<> &doc, rapidxml::xml_node<> &parent);
};
#endif
