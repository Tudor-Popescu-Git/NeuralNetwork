/*
 * NeuronNetwork.cpp
 *
 *  Created on: Nov 8, 2016
 *      Author: root
 */

#include "stdafx.h"
#include <sstream>
#include "../_h/Neuron.h"
#include "../_h/Synapse.h"
#include "../_h/Layer.h"
#include "../_h/LayerSlice.h"
#include "../_h/LayerConnector.h"
#include "../_h/Primitives.h"
#include "../_h/NeuronNetwork.h"
#include "../_h/rapidxml.hpp"
#include "../_h/rapidxml_print.hpp"

using namespace std;
using namespace rapidxml;

int (*NeuronNetwork::activationV)	(vector<double*>& input, vector<double*>& output) = sigmoid_V;
int (*NeuronNetwork::activationDV)	(vector<double*>& input,vector<double*>& output) = sigmoid_DV;
int (*NeuronNetwork::costDV)	(vector<double> &expected, vector<double*> &output, vector<double*> &error) = quadratic_DV;

int(*NeuronNetwork::activation)	(double input, double &output) = sigmoid;
int(*NeuronNetwork::activation_D)	(double input, double &output) = sigmoid_D;
int(*NeuronNetwork::cost_D) (double expected, double output, double &error) = quadratic_D;
/*
template<class InputIt>
RET(*NeuronNetwork::activation_S)		(LayerSlice<InputIt> &input, LayerSlice<InputIt> &output) = sigmoid_S;

template<class InputIt>
RET(*NeuronNetwork::activation_DS)		(LayerSlice<InputIt> &input, LayerSlice<InputIt> &output) = sigmoid_DS;

template<class InputIt>
RET(*NeuronNetwork::cost_DS)			(LayerSlice<InputIt> &expected, LayerSlice<InputIt> &output, LayerSlice<InputIt> &error) = quadratic_DS;
*/
vector<double> NeuronNetwork::expected;

NeuronNetwork::NeuronNetwork(vector<int> &numLayers, double learningFactor_, double momentumFactor_){

	auto sz = numLayers.size();
	auto it = numLayers.begin();

	learningFactor = learningFactor_;
	momentumFactor = momentumFactor_;
	
	expected.reserve((*(numLayers.end() - 1)));
	if (sz <= 1) {
		cerr << "bad number of layers=" << sz << ", will exit" << endl;
		exit(-1);
	}
	UniformLayerConnector ulc;
	Layer *previousLayer = nullptr, *currentLayer = nullptr;
	currentLayer = new Layer(*it++, Types::IN, ulc, learningFactor, momentumFactor);
	layers.push_back(currentLayer);
	previousLayer = currentLayer;
	inputs = currentLayer->inputs;
	while (it != (numLayers.end() - 1)) {
		if (*it == 0) {
			//treat layer with 0 elements, suggested deconstruct nn, or just plain exit
		}
		currentLayer = new Layer(*it++, Types::HIDDEN, ulc, learningFactor, momentumFactor);
		layers.push_back(currentLayer);
		previousLayer->bind(*currentLayer);
		previousLayer = currentLayer;
	}
	currentLayer = new Layer(*it, Types::OUT, ulc, learningFactor, momentumFactor);
	layers.push_back(currentLayer);
	previousLayer->bind(*currentLayer);
	outputs = currentLayer->outputs;
}
NeuronNetwork::NeuronNetwork(const string &filename) {
	loadXML(filename);
}
int NeuronNetwork::activate(vector<double> &inputs) {
	int ret = 0;
	if (inputs.size() != layers[0]->inputs.size()) {
		return -1;
	}
	else {
		auto it = inputs.begin();
		auto it2 = this->inputs.begin();
		while (it != inputs.end()) {
			**it2 = *it;
			it++;
			it2++;
		}
	}
	
	for (auto& layer : layers) {
		ret = layer->activate();
		if (ret != 0) {
			return ret;
		}
	}
	return 0;
}
int NeuronNetwork::backprop(vector<double> &outputs) {
	expected = outputs;
	auto layerIt = layers.rbegin();
	for (; layerIt != layers.rend(); layerIt++) {
		(*layerIt)->deltaPass();
	}
	layerIt = layers.rbegin();
	for (; layerIt != layers.rend(); layerIt++) {
		(*layerIt)->updatePass();
	}
	return 0;
}
double& NeuronNetwork::getLearningFactor() {
	return learningFactor;
}
double& NeuronNetwork::getMomentumFactor() {
	return momentumFactor;
}
RET NeuronNetwork::saveXML(const string &filename) {
	xml_document<> doc;
	auto node = doc.allocate_node(node_element, "NeuronNetwork");
	doc.append_node(node);
	stringstream buffer;
	
	buffer << this->momentumFactor;
	char *momentumStr = doc.allocate_string(buffer.str().c_str());
	auto momentumAttrib = doc.allocate_attribute("momentumFactor", momentumStr);
	//auto node1 = doc.allocate_node(node_element, "momentumFactor", momentumStr);
	node->append_attribute(momentumAttrib);
	//node->append_node(node1);
	buffer.str("");

	buffer << this->learningFactor;
	char *learningStr = doc.allocate_string(buffer.str().c_str());
	//auto node2 = doc.allocate_node(node_element, "learningFactor", learningStr);
	auto learningAttrib = doc.allocate_attribute("learningFactor", learningStr);
	node->append_attribute(learningAttrib);
	//node->append_node(node2);
	buffer.str("");

	for each (auto &layer in layers)
	{
		layer->saveXML(doc, *node);
	}
	ofstream ofs(filename);
	if (ofs) {
		ofs << doc;
	}
	return RET::OK;
}
RET NeuronNetwork::loadXML(const string &filename) {
	ifstream ifs(filename);
	string str;
	if (!ifs) {
		return RET::NOK;
	}
	ifs.seekg(0, std::ios::end);
	size_t size = ifs.tellg();
	
	char * buffer = new char[size + 1]();
	try {
		buffer[size] = 0;
		ifs.seekg(0, std::ios::beg);
		ifs.read(buffer, size);
		xml_document<> doc;
		doc.parse<0>(buffer);
		auto node = doc.first_node("NeuronNetwork");
		auto attrib = node->first_attribute();
		for (auto attr = node->first_attribute(); attr; attr = attr->next_attribute()) {
			if (!strcmp(attr->name(), "momentumFactor")) {
				momentumFactor = atof(attr->value());
				continue;
			}
			if (!strcmp(attr->name(), "learningFactor")) {
				learningFactor = atof(attr->value());
			}
		}
		auto layerNode = node->first_node("Layer");
		XmlLayerConnector xmlLayerConnector;
		for (auto layerNode = node->first_node("Layer"); !strcmp(layerNode->name(), "Layer"); layerNode = layerNode->next_sibling()) {
			layers.push_back(new Layer(xmlLayerConnector, learningFactor, momentumFactor, doc, *layerNode));
		}
	}
	catch (exception &e) {

	}
	delete[] buffer;
	return RET::OK;
}
NeuronNetwork::~NeuronNetwork() {
// TODO Auto-generated destructor stub
	//alot of leaks
}

void xmlWriter::increaseIndent() {
	indent.push_back(INDENT_CHAR);
}
void xmlWriter::decreaseIndent() {
	indent.pop_back();
}
void xmlWriter::writeAttributes(const initializer_list<pair<string, string>> &attributes) {
	string attrib;
	string value;
	auto attrib_beg = attributes.begin();
	auto attrib_end = attributes.end();
	while (attrib_beg != attrib_end) {
		attrib = (*attrib_beg).first;
		value = (*attrib_beg).second;
		f << " " << attrib.c_str() << "=\"" << value.c_str() << "\"";
		attrib_beg++;
	}
}
xmlWriter & xmlWriter::writeBeginElement(const string &element) {
	f << "\n" << indent.c_str() << "<" << element.c_str() << ">";
	increaseIndent();
	tagStack.push(element);
	return *this;
}
xmlWriter & xmlWriter::writeBeginElement(const string &element, const initializer_list<pair<string, string>> &attributes) {
	f << "\n" << indent.c_str() << "<" << element.c_str();
	writeAttributes(attributes);
	f << ">";
	tagStack.push(element);
	increaseIndent();
	return *this;
}
xmlWriter & xmlWriter::writeElement(const string& element, const initializer_list<pair<string, string>> &attributes, const string &value) {
	writeBeginElement(element, attributes);
	f << value.c_str();
	string tag = tagStack.top();
	decreaseIndent();
	f << "</" << tag.c_str() << ">";
	return *this;
}
xmlWriter & xmlWriter::writeValue(const string & value) {

	f << "\n" << indent.c_str() << value.c_str();

	return *this;
}
xmlWriter & xmlWriter::writeEndElement() {
	string tag = tagStack.top();
	decreaseIndent();
	f << "\n" << indent.c_str() << "</" << tag.c_str() << ">";
	return *this;
}

