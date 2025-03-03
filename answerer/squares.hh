/**
 * @file squares.hh
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2025-01-26
 *
 * @copyright Copyright (c) 2025
 */

#ifndef ANSWERER_SQUARES_HH_
#define ANSWERER_SQUARES_HH_

#include "answerer.hh"

class squares : public answerer
{
public:
	int answer(void) override;
	virtual const std::string name() const override { return "squares"; }
};

#endif /* ANSWERER_SQUARES_HH_ */
