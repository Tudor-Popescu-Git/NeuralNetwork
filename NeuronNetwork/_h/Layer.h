#ifndef H_LAYER_H_
#define H_LAYER_H_

#include "../_h/Common.h"
#include "../_h/Neuron.h"
#include "../_h/Layer.h"
#include "../_h/LayerConnector.h"
#include "../_h/LayerSlice.h"
#include "../_h/rapidxml.hpp"
/*
namespace rapidxml {
	class xml_document;
}
*/
class Neuron;
struct Synapse;
class LayerConnector;


class Layer {
	friend class LayerConnector;
	double &learningFactor;
	double &momentumFactor;
	const int NUMBER_OF_SLICES = 4;
public:
	typedef std::vector<Neuron*>::iterator iterator;
	Types type;
	std::vector<Neuron*> neurons;
	std::vector<double*> inputs;
	std::vector<double*> biases;
	std::vector<double*> outputs;
	std::vector<double*> errors;
	std::vector<double*> deltas;

	std::vector<std::vector<Synapse*>*> from, to;
	LayerConnector& connector;
	Layer* nextLayer;
	//vector<vector<Neuron&>>::iterator sliceIterator;
	Layer(unsigned int number, Types type, LayerConnector& connector, double &learningFactor, double &momentumFactor, int NUMBER_OF_SLICES = 4);
	Layer(LayerConnector &connector, double &learningFactor, double &momentumFactor, rapidxml::xml_document<> &doc, rapidxml::xml_node<> &parent);
	Layer& bind(Layer& nextLayer);
	int activate(void);
	int backprop(void);
	int deltaPass(void);
	int updatePass(void);
	RET saveXML(rapidxml::xml_document<> &doc, rapidxml::xml_node<> &parent);
	RET loadXML(rapidxml::xml_document<> &doc, rapidxml::xml_node<> &parent);
	//vector<vector<Neuron&>>::iterator& getSliceIterator(int num);
	iterator begin();
	iterator end();
	size_t size(void);
private:
	vector<LayerSlice<iterator>> slices;
	RET activate_S(LayerSlice<iterator> &);
	RET cost_DS(vector<double> &expected, LayerSlice<iterator> &slice);
	RET activation_DS(LayerSlice<iterator> &input);
	RET hadamardDot_S(LayerSlice<iterator> &slice);
	RET deltaPass_S(LayerSlice<iterator> &slice);
	RET updatePass_S(LayerSlice<iterator> &slice);
	RET slicesDivisor(int numSlices);
};
#endif
