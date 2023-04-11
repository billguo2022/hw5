#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#endif

// add or remove necessary headers as you please
#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;

// Helper function for the schedule function
bool helper(
    const AvailabilityMatrix& availability,
    const size_t dailyReq,
    const size_t maxShiftsAllowed,
    DailySchedule& dailySched,
    std::vector<size_t> workingDays,
    size_t currentDay
);

// Main scheduling function
bool schedule(
    const AvailabilityMatrix& availability,
    const size_t dailyReq,
    const size_t maxShiftsAllowed,
    DailySchedule& dailySched
)
{
    // Return false if there's no availability data
    if (availability.size() == 0U) {
        return false;
    }

    // Clear the daily schedule
    dailySched.clear();

    // Initialize working days counter
    std::vector<size_t> workingDays;
    for (size_t i = 0; i < availability[0].size(); i++){

        workingDays.push_back(0);
		}

    // Initialize daily schedule with empty worker lists
    for (size_t i = 0; i < availability.size(); i++) {
        std::vector<Worker_T> empty;
        dailySched.push_back(empty);
    }

    // Call the helper function to generate the schedule
    return helper(availability, dailyReq, maxShiftsAllowed, dailySched, workingDays, 0);
}

// Helper function to recursively generate the schedule
bool helper(
    const AvailabilityMatrix& availability,
    const size_t dailyReq,
    const size_t maxShiftsAllowed,
    DailySchedule& dailySched,
    std::vector<size_t> workingDays,
    size_t currentDay
)
{
    // Base case: if all days are scheduled, return true
    if (currentDay >= availability.size()) {
        return true;
    }
    // If the current day's schedule is already filled, move on to the next day
    else if (dailySched[currentDay].size() >= dailyReq) {
        return helper(availability, dailyReq, maxShiftsAllowed, dailySched, workingDays, currentDay + 1);
    }
    // If the current day's schedule is not yet filled, try to add workers
    else {
        bool alreadyAssignedToday = false;
        for (Worker_T workerId = 0; workerId < availability[0].size(); workerId++) {
            // Check if the worker is already assigned for the current day
            if (!dailySched[currentDay].empty()) {

                for (size_t i = 0; i < dailySched[currentDay].size(); i++) {

                    if (dailySched[currentDay][i] == workerId)

                        alreadyAssignedToday = true;
                }
            }
            // If the worker is available, hasn't reached max shifts, and is not already assigned, add them to the schedule
            bool x = false;
            if (availability[currentDay][workerId] == 1 && workingDays[workerId] < maxShiftsAllowed
                && !alreadyAssignedToday) {
                dailySched[currentDay].push_back(workerId);
                workingDays[workerId]++;
                // Recursively call the helper function to fill the schedule
                x = helper(availability, dailyReq, maxShiftsAllowed, dailySched, workingDays, currentDay);
                // If the schedule works, return true
                if (x == true)
                    return true;
                // Otherwise, backtrack by removing the worker from the schedule and decrementing their working days
                dailySched[currentDay].pop_back();
                workingDays[workerId]--;
            }
        }
    }
}
    // If no valid