/*
 * NeuronNetwork.h
 *
 *  Created on: Nov 8, 2016
 *      Author: root
 */

#ifndef H_NEURONNETWORK_H_
#define H_NEURONNETWORK_H_

#include "../_h/Common.h"
#include <stack>
#include <fstream>
#include <initializer_list>

//using namespace std;

class Neuron;
class Layer;
template<class InputIt>
class LayerSlice;

class NeuronNetwork {
	friend class Neuron;
	friend class Layer;
	double learningFactor;
	double momentumFactor;
public:
	std::vector<Layer*> layers;
	std::vector<double*> inputs;
	std::vector<double*> outputs;

	static std::vector<double> expected;

	static int (*activationV)	(std::vector<double*>& input, std::vector<double*>& output);
	static int (*activationDV)	(std::vector<double*>& input, std::vector<double*>& output);
	static int (*costDV)		(std::vector<double> &expected, std::vector<double*> &output, std::vector<double*> &error);

	static int (*activation)		(double input, double &output);
	static int (*activation_D)		(double input, double &output);
	static int (*cost_D)			(double expected, double output, double &error);
	/*
	template<class InputIt>
	static RET (*activation_S)		(LayerSlice<InputIt> &input, LayerSlice<InputIt> &output);
	
	template<class InputIt>
	static RET (*activation_DS)		(LayerSlice<InputIt> &input, LayerSlice<InputIt> &output);
	
	template<class InputIt>
	static RET (*cost_DS)			(LayerSlice<InputIt> &expected, LayerSlice<InputIt> &output, LayerSlice<InputIt> &error);
	*/
	NeuronNetwork(std::vector<int>& numLayers, double learningFactor = 0.25, double momentumFactor = 0.0);
	NeuronNetwork(const string &filename);
	//void setInput(vector<double> &&input);
	int activate(std::vector<double> &inputs);
	int backprop(std::vector<double> & outputs);
	double &getLearningFactor();
	double &getMomentumFactor();
	RET saveXML(const std::string &filename);
	RET loadXML(const string &filename);
	virtual ~NeuronNetwork();

};
class xmlWriter {
private:
	stack<string> tagStack;
	fstream f;
	string indent;
	const char INDENT_CHAR = '\t';
	void increaseIndent();
	void decreaseIndent();
	void writeAttributes(const initializer_list<pair<string, string>> &attributes);
public:
	xmlWriter(const string &filename) : f(filename, fstream::trunc) {
		if (!f) {
			f.open("ceva.xml");
			bool breakp = true;
		}
	}
	~xmlWriter() { f.close(); }
	xmlWriter(const xmlWriter &xmlWriter) = delete;
	xmlWriter(const xmlWriter &&xmlWriter) = delete;

	xmlWriter & writeBeginElement(const string& element);
	xmlWriter & writeBeginElement(const string& element, const initializer_list<pair<string,string>> &attributes);
	xmlWriter & writeValue(const string &value);
	xmlWriter & writeEndElement();
	xmlWriter & writeElement(const string& element, const initializer_list<pair<string, string>> &attributes, const string &value);
};
#endif /* CPP_NEURONNETWORK_H_ */
