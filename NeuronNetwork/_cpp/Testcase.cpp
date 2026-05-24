/*
 * Testcase.cpp
 *
 *  Created on: Nov 22, 2016
 *      Author: root
 */

#include "stdafx.h"
#include <chrono>
#include "../_h/Barrier.h"
#include "../_h/Common.h"
#include "../_h/Neuron.h"
#include "../_h/Synapse.h"
#include "../_h/Layer.h"
#include "../_h/Primitives.h"
#include "../_h/NeuronNetwork.h"
#include "../_h/Testcase.h"
#include "../_h/Threadpool.h"
#include <sstream>

using namespace std;
using namespace mythreading;
void Testcase::testStructure() {
	int i = 0;
	vector<int> Layers = { 2, 3, 4 };
	NeuronNetwork nn(Layers);
	for (auto& layersIt : nn.layers) {
		auto layerIt = (*layersIt).begin();
		cout << "Layer " << i++ << endl;
		while (layerIt != (*layersIt).end()) {
			Neuron *current = *layerIt;
			cout << "Neuron with ID " << current->ID << " has synapses:"
				<< endl;
			auto synapsesIt = current->from.begin();
			while (synapsesIt != current->from.end()) {
				if ((*synapsesIt)->from)
					cout << "From ID" << (*synapsesIt)->from->ID << endl;
				synapsesIt++;
			}
			synapsesIt = current->to.begin();
			while (synapsesIt != current->to.end()) {
				if ((*synapsesIt)->to)
					cout << "To ID" << (*synapsesIt)->to->ID << endl;
				synapsesIt++;
			}
			layerIt++;
		}
	}
}
void Testcase::testInput() {
	vector<int> Layers = { 5, 6, 7 };
	vector<double> input = { 1.2, 2.3, 1.1, 1.2, 1.3};
	NeuronNetwork nn(Layers);
	nn.activate(input);
}
void Testcase::testIO(void){
	vector<int> Layers = { 1, 1, 1 };
	vector<double> input = { 0.1 };
	vector<double> output = { 0.9 };
	NeuronNetwork nn(Layers, 0.25, 0.9);
	for (int i = 0; i < 100; i++) {
		nn.activate(input);
		nn.backprop(output);
	}
}
void Testcase::testDiviser(void) {
	int NUM_NEURONS = 6;
	int NUM_SLICES = 7;
	int CURRENT_SLICE = 0;
	int CURRENT_BEGIN = 0;
	int CURRENT_END = 0;
	vector<int> sliceIndexes;
	sliceIndexes.reserve(NUM_SLICES);
	
	int a = NUM_NEURONS / NUM_SLICES;
	int b = NUM_NEURONS % NUM_SLICES;

	for (int i = 0; i < NUM_SLICES; i++) {
		if (b > 0) {
			sliceIndexes.push_back(a + 1);
			b--;
		}
		else {
			sliceIndexes.push_back(a);
		}
		

		CURRENT_SLICE = ceil((double)(NUM_NEURONS - CURRENT_SLICE)/ (NUM_SLICES-i))+CURRENT_SLICE;
		CURRENT_END += CURRENT_SLICE;
		cout << "CURRENT_SLICE = " << CURRENT_SLICE << " CURRENT_BEGIN = " << CURRENT_BEGIN << "CURRENT_END = " << CURRENT_END;
		CURRENT_BEGIN += CURRENT_SLICE;
	}

}
void Testcase::doTests(void) {
	//testInput();
	//testIO();
	//testBarrier();
	//testThreadpool();
	//testDiviser();
	//testXmlWriter();
	testRapidXml();
}
void localSignalAndWait(barrier &b) {
	for (int i = 0; i < 1000; i++) {
		b.wait();
	}
}
void Testcase::testBarrier(void) {
	vector<thread> threads;
	int numThreads = 10;
	barrier b(numThreads);
	auto f = [&]() {for(int i = 0; i < 1000; i++) b.wait(); };
	for (int i = 0; i < numThreads - 1; i++) {
		threads.emplace_back(f);
	}

	for (int i = 0; i < 999; i++) {
		b.wait();
	}
	b.wait();
	for (int i = 0; i < numThreads - 1; i++) {
		threads[i].join();
	}
}
void Testcase::testThreadpool(void) {
	std::chrono::high_resolution_clock clock;
	auto start = clock.now(); 
	{
		const int NUM_THREADS = 4;
		const int MULTIPLIER = 20;
		threadpool tp(NUM_THREADS);
		for (int i = 0; i < NUM_THREADS * MULTIPLIER; i++) {
			tp.enqueueWork([]() 
			{
				this_thread::sleep_for(std::chrono::milliseconds(100));
			});
		}
	}
	cout << "finished in:" << std::chrono::duration_cast<std::chrono::milliseconds>(clock.now() - start).count();
	}
void Testcase::testXmlWriter(void) {
	xmlWriter writer("ceva.xml");
	writer.writeBeginElement("NeuronNetwork", { { "numLayers" , "3" },{ "momentum", "0.1" } }).writeBeginElement("Layer").writeValue("1234").writeEndElement().writeEndElement();
	writer.writeElement("NeuronNetwork2", { { "numLayers" , "3" },{ "momentum", "0.1" } }, "val");
}
void Testcase::testRapidXml(void) {
	vector<int> Layers = { 1,1,1 };
	vector<double> input = { 0.1 };
	vector<double> output = { 0.9 };
	NeuronNetwork nn(Layers, 0.25, 0.9);
	nn.saveXML("saved.xml");
	NeuronNetwork nn1("saved.xml");
}
int main(void) {
	Testcase t;
	t.doTests();
}

