#ifndef H_LAYERCONNECTOR_H_
#define H_LAYERCONNECTOR_H_
#include "Common.h"
#include "Layer.h"
class LayerConnector {
public:
	virtual void connect(Layer &from, Layer &to) = 0;
};
class UniformLayerConnector: public LayerConnector {
	friend class Layer;
	double weightSum = 1.0;
	void connect(Layer &from, Layer &to) override;

};
class XmlLayerConnector : public LayerConnector {
	friend class Layer;
	void connect(Layer &from, Layer &to) override;
};
#endif