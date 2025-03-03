/**
 * @file donut.hh
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2025-01-26
 *
 * @copyright Copyright (c) 2025
 */

#ifndef ANSWERER_DONUT_HH_
#define ANSWERER_DONUT_HH_

#include "answerer.hh"

class donut : public answerer
{
public:
	int answer(void) override;
	virtual const std::string name() const override { return "donut"; }
};

#endif /* ANSWERER_DONUT_HH_ */
