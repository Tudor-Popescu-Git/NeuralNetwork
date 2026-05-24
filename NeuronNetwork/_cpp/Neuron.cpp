/*
 * Neuron.cpp
 *
 *  Created on: Nov 8, 2016
 *      Author: root
 */

#include "stdafx.h"
#include <sstream>
#include "../_h/Neuron.h"
#include "../_h/rapidxml.hpp"

using namespace rapidxml;
int Neuron::num = 0;
Neuron::Neuron(xml_document<> &doc, xml_node<> &parent) {
	loadXML(doc, parent);
}
Synapse* Neuron::bind(Neuron* neuron, double weight) {
	Synapse* s = new Synapse(this, neuron, weight);
	this->to.push_back(s);
	neuron->from.push_back(s);
	return s;
}
void Neuron::activate(void) {
	double input = 0;
	for (Synapse* s : from) {
		input += s->from->output * s->weight;
	}
	//NeuronNetwork::activationFunction(input, bias, this->output);
}

Neuron::~Neuron() {
	// TODO Auto-generated destructor stub
}

RET Neuron::saveXML(xml_document<> &doc, xml_node<> &parent){
	auto node = doc.allocate_node(node_element, "Neuron");
	parent.append_node(node);
	stringstream buffer;

	buffer << ID;
	char *IDStr = doc.allocate_string(buffer.str().c_str());
	auto IDAttrib = doc.allocate_attribute("ID", IDStr);
	node->append_attribute(IDAttrib);
	buffer.str("");

	buffer << bias;
	char *biasStr = doc.allocate_string(buffer.str().c_str());
	auto biasAttrib = doc.allocate_attribute("bias", biasStr);
	node->append_attribute(biasAttrib);
	buffer.str("");

	for (auto &synapse : to) {
		synapse->saveXML(doc, *node);
	}
	
	return RET::OK;
}
RET Neuron::loadXML(xml_document<> &doc, xml_node<> &parent) {
	if (strcmp(parent.name(), "Neuron")) {
		return RET::NOK;
	}
	auto attrib = parent.first_attribute();
	for (; attrib; attrib = attrib->next_attribute()) {
		if (!strcmp(attrib->name(), "ID")) {
			ID = atoi(attrib->value());
		}
		if (!strcmp(attrib->name(), "bias")) {
			bias = atof(attrib->value());
		}
	}
	return RET::OK;
}