/**
 * @file beacon.hh
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2025-01-26
 *
 * @copyright Copyright (c) 2025
 */

#ifndef ANSWERER_BEACON_HH_
#define ANSWERER_BEACON_HH_

#include "answerer.hh"

class beacon : public answerer
{
public:
	int answer(void) override;
};

#endif /* ANSWERER_BEACON_HH_ */
