#include "SingleAgentSolver.h"


double SingleAgentSolver::compute_h_value(const BasicGraph& G, int curr, int goal_id,
                             const vector<pair<int, int> >& goal_location) const
{
    if (goal_id < 0 || goal_id >= (int)goal_location.size())
        return 0;

    int target_loc = goal_location[goal_id].first;
    double h = 0;
    auto it = G.heuristics.find(target_loc);
    if (it != G.heuristics.end() && curr >= 0 && curr < (int)it->second.size() && it->second[curr] < INT_MAX)
    {
        h = it->second[curr];
    }
    else
    {
        h = G.get_Manhattan_distance(curr, target_loc);
    }

    goal_id++;
    while (goal_id < (int) goal_location.size())
    {
        int next_target = goal_location[goal_id].first;
        int prev_target = goal_location[goal_id - 1].first;
        auto it2 = G.heuristics.find(next_target);
        if (it2 != G.heuristics.end() && prev_target >= 0 && prev_target < (int)it2->second.size() && it2->second[prev_target] < INT_MAX)
        {
            h += it2->second[prev_target];
        }
        else
        {
            h += G.get_Manhattan_distance(prev_target, next_target);
        }
        goal_id++;
    }
    return h;
}
