/**
 * @file battle.hh
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2025-01-26
 *
 * @copyright Copyright (c) 2025
 */

#ifndef ANSWERER_BATTLE_HH_
#define ANSWERER_BATTLE_HH_

#include "answerer.hh"

class battle : public answerer
{
public:
	int answer(void) override;
	virtual const std::string name() const override { return "battle"; }
};

#endif /* ANSWERER_BATTLE_HH_ */
