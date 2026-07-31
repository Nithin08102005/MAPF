#pragma once
#include "common.h"
#include "States.h"

#define WEIGHT_MAX INT_MAX/2


class BasicGraph
{
public:
    vector<std::string> types;
    unordered_map<int, vector<double>> heuristics;
    virtual ~BasicGraph()= default;
    string map_name;
	virtual bool load_map(string fname) = 0;
    list<State> get_neighbors(const State& v) const;
    list<int> get_neighbors(int v) const;
    list<State> get_reverse_neighbors(const State& v) const; // ignore time
    double get_weight(int from, int to) const; // fiducials from and to are neighbors
    vector<vector<double> > get_weights() const {return weights; }
    int get_rotate_degree(int dir1, int dir2) const; // return 0 if it is 0; return 1 if it is +-90; return 2 if it is 180

    void print_map() const;
    int get_rows() const { return rows; }
    int get_cols() const { return cols; }
    int size() const { return rows * cols; }

    bool valid_move(int loc, int dir) const {return (weights[loc][dir] < WEIGHT_MAX - 1); }
    int get_Manhattan_distance(int loc1, int loc2) const;
    int move[4];
    void copy(const BasicGraph& copy);
    int get_direction(int from, int to) const;

	vector<double> compute_heuristics(int root_location); // compute distances from all lacations to the root location
	bool load_heuristics_table(std::ifstream& myfile);
	void save_heuristics_table(string fname);

    void get_occupied_cells(int loc, int orientation, int out_cells[3]) const
    {
        out_cells[0] = loc;
        if (orientation >= 0 && orientation < 4)
        {
            out_cells[1] = loc + move[orientation];
            out_cells[2] = loc - move[orientation];
        }
        else
        {
            out_cells[1] = loc;
            out_cells[2] = loc;
        }
    }

    void get_5cell_occupied_cells(int loc, int orientation, int out_cells[5]) const
    {
        out_cells[0] = loc;
        if (orientation >= 0 && orientation < 4)
        {
            out_cells[1] = loc + move[orientation];
            out_cells[2] = loc - move[orientation];
            out_cells[3] = loc + 2 * move[orientation];
            out_cells[4] = loc - 2 * move[orientation];
        }
        else
        {
            out_cells[1] = loc;
            out_cells[2] = loc;
            out_cells[3] = loc;
            out_cells[4] = loc;
        }
    }

    bool is_cell_valid_for_robot(int cell) const
    {
        if (cell < 0 || cell >= rows * cols)
            return false;
        if (types[cell] == "Obstacle" || types[cell] == "Endpoint")
            return false;
        return true;
    }

    bool valid_3cell_state(int loc, int orientation) const
    {
        if (loc < 0 || loc >= rows * cols)
            return false;
        if (!is_cell_valid_for_robot(loc))
            return false;
        if (orientation >= 0 && orientation < 4)
        {
            int front = loc + move[orientation];
            int back = loc - move[orientation];
            if (get_Manhattan_distance(loc, front) != 1 || !is_cell_valid_for_robot(front))
                return false;
            if (get_Manhattan_distance(loc, back) != 1 || !is_cell_valid_for_robot(back))
                return false;
            if (orientation == 1 || orientation == 3)
            {
                int col = loc % cols;
                if (col > 0 && col < cols - 1)
                {
                    if (!is_cell_valid_for_robot(loc - 1) || !is_cell_valid_for_robot(loc + 1))
                        return false;
                }
            }
        }
        return true;
    }

    bool has_valid_3cell_orientation(int loc) const
    {
        for (int dir = 0; dir < 4; ++dir)
        {
            if (valid_3cell_state(loc, dir))
                return true;
        }
        return false;
    }

    // Returns true if a 3-cell robot centered at loc has AT LEAST 2 valid
    // orientations, meaning it can arrive from one direction and turn/leave
    // in another. Cells with only 1 valid orientation are dead-ends: a robot
    // can enter but cannot turn to exit, so they must never be used as goals.
    bool is_robot_maneuverable(int loc) const
    {
        int count = 0;
        for (int dir = 0; dir < 4; ++dir)
        {
            if (valid_3cell_state(loc, dir))
                ++count;
        }
        return count >= 2;
    }

    // Checks ALL 9 cells of the 3x3 area swept during an in-place 90-degree rotation.
    // A 3-cell robot sweeps a 3x3 footprint when turning: before it occupies 3 cells in
    // one axis, after it occupies 3 cells in the perpendicular axis. Together they form a
    // 3x3 grid around the center cell. All 9 must be valid travel cells for the turn to
    // be physically safe. This restricts turns to the CENTER cell of a 3-wide corridor.
    bool valid_3cell_rotation(int loc, int from_ori, int to_ori) const
    {
        if (!valid_3cell_state(loc, from_ori)) return false;
        if (!valid_3cell_state(loc, to_ori))   return false;
        int row = loc / cols;
        int col = loc % cols;
        // Check all 9 cells in the 3x3 block around center.
        // It should not turn if any endpoint or obstacle is in its 9 proximity cells.
        for (int dr = -1; dr <= 1; ++dr)
        {
            for (int dc = -1; dc <= 1; ++dc)
            {
                int r = row + dr;
                int c = col + dc;
                if (r < 0 || r >= rows || c < 0 || c >= cols)
                    return false;
                int cell = r * cols + c;
                if (!is_cell_valid_for_robot(cell))
                    return false;
                if (types[cell] == "Endpoint")
                    return false;
            }
        }
        return true;
    }

    int rows;
    int cols;
    vector<vector<double> > weights; // (directed) weighted 4-neighbor grid
    bool consider_rotation;
};
