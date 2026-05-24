/*
 * Synapse.cpp
 *
 *  Created on: Nov 8, 2016
 *      Author: root
 */

#include "stdafx.h"
#include <sstream>
#include "../_h/Synapse.h"
#include "../_h/rapidxml.hpp"

using namespace rapidxml;

RET Synapse::saveXML(xml_document<> &doc, xml_node<> &parent) {
	auto node = doc.allocate_node(node_element, "Synapse");
	parent.append_node(node);
	stringstream buffer;

	buffer << to->ID;
	char *IDStr = doc.allocate_string(buffer.str().c_str());
	//auto IDNode = doc.allocate_node(node_element, "ID", IDStr);
	auto IDAttrib = doc.allocate_attribute("ID", IDStr);
	//node->append_node(IDNode);
	node->append_attribute(IDAttrib);
	buffer.str("");

	buffer << weight;
	char *weightStr = doc.allocate_string(buffer.str().c_str());
	//auto weightNode = doc.allocate_node(node_element, "Weight", weightStr);
	auto weightAttrib = doc.allocate_attribute("Weight", weightStr);
	node->append_attribute(weightAttrib);
	//node->append_node(weightNode);
	buffer.str("");

	return RET::OK;
}