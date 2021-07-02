#include "timer.hpp"
 
timer::timer() : m_beg(clock_t::now())
{
}
	
void timer::reset()
{
  m_beg = clock_t::now();
}
	
double timer::elapsed() const
{
  return std::chrono::duration_cast<second_t>(clock_t::now() - m_beg).count();
}
