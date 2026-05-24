/*
 * Neuron.h
 *
 *  Created on: Oct 24, 2016
 *      Author: root
 */
#ifndef H_NEURON_H_
#define H_NEURON_H_

#include "../_h/Common.h"
#include "../_h/Synapse.h"
#include "../_h/NeuronNetwork.h"
#include "../_h/rapidxml.hpp"

/*
 enum NeuronStates {
 READY, COMPUTING, NOTREADY
 };
 */
struct Synapse;

class Neuron {
private:

public:
	static int num;
	int ID;
	double c = 1.0;
	double input = 0.0;
	double bias = 0.0;
	double output = 0.0;
	
	union {
		double delta;
		double error = 0.0;
	};
	
	Types neuronType;
//	NeuronStates neuronState;
	vector<Synapse*> to;
	vector<Synapse*> from;
	Neuron(Types neuronType = IN, double c = 1.0) :
			ID(num++), c(c), neuronType(
					neuronType) {
	} //, neuronState(NOTREADY) {}
	Neuron(rapidxml::xml_document<> &doc, rapidxml::xml_node<> &parent);
	Synapse* bind(Neuron* neuron, double weight = 0.5);
	void activate(void);
	virtual ~Neuron();
	RET saveXML(rapidxml::xml_document<> &doc, rapidxml::xml_node<> &parent);
	RET loadXML(rapidxml::xml_document<> &doc, rapidxml::xml_node<> &parent);
};
#endif

