#pragma once
#include <thread>
#include "../_h/Neuron.h"
template<class InputIt>
struct LayerSlice {
private:
	friend class Layer;
	Layer &_layer;
	InputIt _begin;
	InputIt _end;
	int startIndex;
public:
	LayerSlice(Layer &layer, InputIt _begin, InputIt _end) :_layer(layer), _begin(_begin), _end(_end), startIndex(_begin - layer.begin()) {}
	size_t size() { return _end - _begin; }
	InputIt cbegin() const { return const_cast(_begin); }
	InputIt cend() const { return const_cast(_end); }
	InputIt begin() { return _begin; }
	InputIt end() { return _end; }
};