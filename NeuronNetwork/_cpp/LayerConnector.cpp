
#include "stdafx.h"
#include "../_h/LayerConnector.h"

void UniformLayerConnector::connect(Layer &from, Layer &to) {
	auto toSize = to.size();
	double weight = 1;//from.size();
	for (auto fromIt = from.begin(); fromIt != from.end(); fromIt++) {
		for (auto toIt = to.begin(); toIt != to.end(); toIt++) {
			(*fromIt)->bind(*toIt, weight);
		}
	}
}
void XmlLayerConnector::connect(Layer &from, Layer &to) {

}