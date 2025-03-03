/**
 * @file pyramid.hh
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2025-01-26
 *
 * @copyright Copyright (c) 2025
 */

#ifndef ANSWERER_PYRAMID_HH_
#define ANSWERER_PYRAMID_HH_

#include "answerer.hh"

class pyramid : public answerer
{
public:
	int answer(void) override;
	virtual const std::string name() const override { return "pyramid"; }
};

#endif /* ANSWERER_PYRAMID_HH_ */
