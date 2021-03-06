/*
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "CarbonEngine/Math/Color.h"
#include "CarbonEngine/Math/Vec3.h"

namespace Carbon
{

/**
 * This class holds a single intersection result generated by intersecting a ray with an entity.
 */
class CARBON_API IntersectionResult
{
public:

    IntersectionResult() {}

    /**
     * Constructs this intersection result with the given values and doesn't report a surface color.
     */
    IntersectionResult(float distance, const Vec3& point, const Vec3& normal, Entity* entity, String material)
        : distance_(distance), point_(point), normal_(normal), entity_(entity), material_(std::move(material))
    {
    }

    /**
     * Constructs this intersection result with the given values including a surface color.
     */
    IntersectionResult(float distance, const Vec3& point, const Vec3& normal, Entity* entity, String material,
                       const Color& surfaceColor)
        : distance_(distance),
          point_(point),
          normal_(normal),
          entity_(entity),
          material_(std::move(material)),
          hasSurfaceColor_(true),
          surfaceColor_(surfaceColor)
    {
    }

    /**
     * Automatic boolean conversion that indicates whether this intersection result contains a valid intersection.
     */
    operator bool() const { return entity_ != nullptr; }

    /**
     * Comparison operator that allows sorting by the intersection distance.
     */
    bool operator<(const IntersectionResult& intersection) const { return distance_ < intersection.getDistance(); }

    /**
     * Returns the intersection distance. This is the distance from the start of the ray to the intersection point.
     */
    float getDistance() const { return distance_; }

    /**
     * Returns the intersection point.
     */
    const Vec3& getPoint() const { return point_; }

    /**
     * Returns the surface normal at the intersection point.
     */
    const Vec3& getNormal() const { return normal_; }

    /**
     * Returns the entity that was intersected.
     */
    Entity* getEntity() const { return entity_; }

    /**
     * Returns the material that was intersected.
     */
    const String& getMaterial() const { return material_; }

    /**
     * Returns whether or not this intersection result knows the surface color where this intersection occurred.
     */
    bool hasSurfaceColor() const { return hasSurfaceColor_; }

    /**
     * Returns the surface color at the intersection point. This is found by looking up the surface's primary texture at
     * the point of intersection, and is only set in some intersections. The return value from
     * IntersectionResult::hasSurfaceColor() indicates whether this intersection result knows the surface color at the
     * intersection point or not.
     */
    const Color& getSurfaceColor() const { return surfaceColor_; }

    /**
     * Returns a human-readable string describing this intersection result, this is useful when debugging.
     */
    operator UnicodeString() const
    {
        if (!entity_)
            return "No intersection";

        return UnicodeString() << "Intersection at distance: " << distance_ << ", point: " << point_
                               << ", normal: " << normal_ << (material_.length() ? (", material: " + material_) : "")
                               << ", entity: " << entity_
                               << (hasSurfaceColor_ ? (", surface color: " + UnicodeString(surfaceColor_)) : "");
    }

private:

    float distance_ = 0.0f;
    Vec3 point_;
    Vec3 normal_;
    Entity* entity_ = nullptr;
    String material_;

    bool hasSurfaceColor_ = false;
    Color surfaceColor_;
};

/**
 * This stream concatenation overload is required in order to be able to directly log an IntersectionResult instance,
 * and is needed because IntersectionResult has automatic conversions to both bool and UnicodeString meaning the
 * implicit conversion to a StringBase<T> is ambiguous.
 */
template <typename T> StringBase<T>& operator<<(StringBase<T>& s, const IntersectionResult& result)
{
    return s << result.operator UnicodeString();
}

}
