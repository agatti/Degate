/**
 * This file is part of the IC reverse engineering tool Degate.
 *
 * Copyright 2008, 2009, 2010 by Martin Schobert
 * Copyright 2012 Robert Nitsch
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

#include "Line.h"
#include "Globals.h"
#include <cmath>
#include <cstdlib>
#include <algorithm>

using namespace degate;

Line::Line() :
    from_x(0),
    from_y(0),
    to_x(0),
    to_y(0),
    diameter(0),
    d_x(0),
    d_y(0)
{
    calculate_bounding_box();
}

Line::Line(float from_x, float from_y, float to_x, float to_y, unsigned int diameter) :
    from_x(from_x),
    from_y(from_y),
    to_x(to_x),
    to_y(to_y),
    diameter(diameter),
    d_x(to_x - from_x),
    d_y(to_y - from_y)
{
    calculate_bounding_box();
}

void Line::clone_deep_into(DeepCopyable_shptr dest, oldnew_t* oldnew) const
{
    auto clone = std::dynamic_pointer_cast<Line>(dest);
    clone->from_x = from_x;
    clone->from_y = from_y;
    clone->to_x = to_x;
    clone->to_y = to_y;
    clone->diameter = diameter;
    clone->d_x = d_x;
    clone->d_y = d_y;
    clone->calculate_bounding_box();
}

float Line::distance_to_line(Point const& p) const
{
    /**
     *         C
     *       / |
     *    /    |
     *   ----------------
     *   A     H        B
     *
     *  AB.AC = |AB|*|AH|
     *
     *  Pythagore :
     *  AC^2 = AH^2 + HC^2 <=> HC = sqrt(AC^2-AH^2)
     *
     */

    Point A(from_x, from_y);
    Point B(to_x, to_y);

    if (from_x == to_x && from_y == to_y)
    {
        return A.get_distance(p);
    }

    // Vector AB.
    Point AB(to_x - from_x, to_y - from_y);

    // Vector AC.
    Point AC(p.get_x() - from_x, p.get_y() - from_y);

    // Scalar AB.AC
    float AB_dot_AC = AB.get_x() * AC.get_x() + AB.get_y() * AC.get_y();

    // If AB.AC is negative then the point C is behind the start point A.
    if (AB_dot_AC < 0)
    {
        return A.get_distance(p);
    }

    // The squared length of AB.
    float AB_squared = AB.get_x() * AB.get_x() + AB.get_y() * AB.get_y();

    // The squared length of AC.
    float AC_squared = AC.get_x() * AC.get_x() + AC.get_y() * AC.get_y();

    // If AB.AC is superior than AB^2 then the point C is after the end point B.
    if (AB_dot_AC > AB_squared)
    {
        return B.get_distance(p);
    }

    // AB.AC = |AB|*|AH| => AB.AC * AB.AC / |AB|^2 = |AB|*|AH| * |AB|*|AH| / |AB|^2 = |AH|^2
    float AH_squared = AB_dot_AC * AB_dot_AC / AB_squared;
    return sqrt(AC_squared - AH_squared);
}

bool Line::in_shape(float x, float y, float max_distance) const
{
    if (is_vertical() || is_horizontal())
    {
        return bounding_box.in_shape(x, y, max_distance);
    }
    else
    {
        if (distance_to_line(Point(x, y)) <= diameter / 2.0 + max_distance)
            return true;
        else
            return false;
    }
}

bool Line::in_bounding_box(BoundingBox const& bbox) const
{
    return bounding_box.in_bounding_box(bbox);
}


BoundingBox const& Line::get_bounding_box() const
{
    return bounding_box;
}

bool Line::is_vertical() const
{
    return to_x - from_x == 0;
}

bool Line::is_horizontal() const
{
    return to_y - from_y == 0;
}

void Line::set_diameter(unsigned int diameter)
{
    this->diameter = diameter;
    calculate_bounding_box();
}

unsigned int Line::get_diameter() const
{
    return diameter;
}

float Line::get_from_x() const
{
    return from_x;
}

float Line::get_from_y() const
{
    return from_y;
}

float Line::get_to_x() const
{
    return to_x;
}

float Line::get_to_y() const
{
    return to_y;
}

void Line::set_from_x(float from_x)
{
    this->from_x = from_x;
    d_x = to_x - from_x;
    calculate_bounding_box();
}

void Line::set_to_x(float to_x)
{
    this->to_x = to_x;
    d_x = to_x - from_x;
    calculate_bounding_box();
}

void Line::set_from_y(float from_y)
{
    this->from_y = from_y;
    d_y = to_y - from_y;
    calculate_bounding_box();
}

void Line::set_to_y(float to_y)
{
    this->to_y = to_y;
    d_y = to_y - from_y;
    calculate_bounding_box();
}

void Line::shift_y(float delta_y)
{
    from_y += delta_y;
    to_y += delta_y;
    calculate_bounding_box();
}

void Line::shift_x(float delta_x)
{
    from_x += delta_x;
    to_x += delta_x;
    calculate_bounding_box();
}


void Line::calculate_bounding_box()
{
    float radius = diameter / 2.0f;

    if (is_vertical())
        bounding_box = BoundingBox(std::max(from_x - radius, 0.f),
                                   std::max(to_x + radius, 0.f), from_y, to_y);
    else if (is_horizontal())
        bounding_box = BoundingBox(from_x, to_x,
                                   std::max(from_y - radius, 0.f),
                                   std::max(to_y + radius, 0.f));
    else
        bounding_box = BoundingBox(from_x, to_x, from_y, to_y);
}


float Line::get_length() const
{
    float dif_x = to_x - from_x;
    float dif_y = to_y - from_y;

    return std::sqrt((dif_x * dif_x) + (dif_y * dif_y));
}

Point Line::get_p1() const
{
    return Point(from_x, from_y);
}

Point Line::get_p2() const
{
    return Point(to_x, to_y);
}

void Line::set_p1(Point const& p)
{
    set_from_x(p.get_x());
    set_from_y(p.get_y());
}

void Line::set_p2(Point const& p)
{
    set_to_x(p.get_x());
    set_to_y(p.get_y());
}
