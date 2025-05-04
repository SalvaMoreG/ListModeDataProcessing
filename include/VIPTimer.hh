
#ifndef __VipTimer_H__
#define __VipTimer_H__

#include <chrono>
#include <ctime>    
#include <string>    

#pragma once

// RETURNS SOME RIDICULOUS TIME (in seconds)
// Calling it twice and subtracting both values gives the time difference in SECONDS

namespace VIP_TIMER
{
 inline std::time_t GetTime()
 {
   	// auto end = std::chrono::system_clock::now();
   	// std::time_t end_time = std::chrono::system_clock::to_time_t(end);
   	// std::cout << in_text << " current time: " << std::ctime(&end_time) ;

	std::time_t resulttime = std::time(nullptr);
	// std::cout << in_text << " time: " << resulttime << std::endl;
	
	return resulttime;
 }
}

#endif
