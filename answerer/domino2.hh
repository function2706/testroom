/**
 * @file domino2.hh
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2025-01-26
 *
 * @copyright Copyright (c) 2025
 */

#ifndef ANSWERER_DOMINO2_HH_
#define ANSWERER_DOMINO2_HH_

#include "answerer.hh"

class domino2 : public answerer
{
public:
	int answer(void) override;
	virtual const std::string name() const override { return "domino2"; }
};

#endif /* ANSWERER_DOMINO2_HH_ */
