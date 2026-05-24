/*
 * Testcase.h
 *
 *  Created on: Nov 22, 2016
 *      Author: root
 */

#ifndef H_TESTCASE_H_
#define H_TESTCASE_H_

#include "..\_h\Common.h"

class Testcase {
public:
	void doTests(void);
	void testStructure(void);
	void testInput(void);
	void testIO(void);
	void testBarrier(void);
	void testThreadpool(void);
	void testDiviser(void);
	void testXmlWriter(void);
	void testRapidXml(void);
};

#endif /* H_TESTCASE_H_ */
