/**
 * @file answerer.hh
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2025-01-26
 *
 * @copyright Copyright (c) 2025
 */

#ifndef ANSWERER_ANSWERER_HH_
#define ANSWERER_ANSWERER_HH_

#include <string>

class answerer
{
public:
	virtual ~answerer() {}

	virtual int answer(void) = 0;
	virtual const std::string name() const = 0;
};

#endif /* ANSWERER_ANSWERER_HH_ */
