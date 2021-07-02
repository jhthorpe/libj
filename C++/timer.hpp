//timer class from learncpp.com

#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono> // for std::chrono functions

 
class timer
{
private:
	// Type aliases to make accessing nested type easier
	using clock_t = std::chrono::high_resolution_clock;
	using second_t = std::chrono::duration<double, std::ratio<1> >;
	
	std::chrono::time_point<clock_t> m_beg;
 
public:
	timer(); 
	void reset();
	double elapsed() const;
};
#endif
