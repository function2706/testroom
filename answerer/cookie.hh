/**
 * @file cookie.hh
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2025-01-26
 *
 * @copyright Copyright (c) 2025
 */

#ifndef ANSWERER_COOKIE_HH_
#define ANSWERER_COOKIE_HH_

#include "answerer.hh"

class cookie : public answerer
{
public:
	int answer(void) override;
	virtual const std::string name() const override { return "cookie"; }
};

#endif /* ANSWERER_COOKIE_HH_ */
