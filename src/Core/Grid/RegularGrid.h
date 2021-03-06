/**
 * This file is part of the IC reverse engineering tool Degate.
 *
 * Copyright 2008, 2009, 2010 by Martin Schobert
 * Copyright 2019-2020 Dorian Bachelot
 *
 * Degate is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * Degate is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with degate. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef __REGULARGRID_H__
#define __REGULARGRID_H__

#include "Grid.h"
#include "Globals.h"

#include <algorithm>
#include <memory>

namespace degate
{
    /**
     * Represents a grid with equidistant spacing between grid lines.
     */
    class RegularGrid : public Grid
    {
    private:
        double distance;
        int min, max;
        grid_set grid_offsets;

        void precalc_steps();
    public:
        RegularGrid(Grid::ORIENTATION orientation) : Grid(orientation)
        {
            min = 0;
            max = 0;
            distance = 0;
        }

        virtual ~RegularGrid()
        {
        }

        virtual grid_iter begin() const { return grid_offsets.begin(); }
        virtual grid_iter end() const { return grid_offsets.end(); }

        virtual void clear()
        {
            distance = 0;
            min = max = 0;
            grid_offsets.clear();
        }

        virtual int get_min() const { return min; }
        virtual int get_max() const { return max; }

        virtual void set_range(int min, int max)
        {
            this->min = std::min(min, max);
            this->max = std::max(min, max);
            precalc_steps();
        }

        virtual void set_distance(double distance)
        {
            assert(distance >= 0);
            this->distance = abs(static_cast<long>(distance));
            precalc_steps();
        }

        virtual double get_distance() const { return distance; }
        virtual int snap_to_grid(int pos) const;
    };

    typedef std::shared_ptr<RegularGrid> RegularGrid_shptr;
}

#endif
