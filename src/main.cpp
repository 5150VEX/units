#include "main.h"
#include "units/Pose.hpp"
#include "units/Temperature.hpp"
#include "units/Vector2D.hpp"
#include "units/Vector3D.hpp"

constexpr int r2i(double value) { return static_cast<int>(value >= 0.0 ? value + 0.5 : value - 0.5); }

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
    units::AccelerationPose a(1_mps2, 2_mps2);
    Number num = Number(1.0);
    num = Number(0.0);
    a.orientation = Quantity<std::ratio<0>, std::ratio<0>, std::ratio<-2>, std::ratio<0>, std::ratio<1>, std::ratio<0>,
                             std::ratio<0>, std::ratio<0>>(1.0);
    a.orientation += 2_rpm2;
    2_rpm2 -= a.orientation;
    Quantity<std::ratio<0>, std::ratio<0>, std::ratio<1>, std::ratio<0>, std::ratio<1>, std::ratio<0>, std::ratio<0>,
             std::ratio<0>>
        c = Multiplied<Angle, Time>();
    Length x = unit_cast<Length>(num);
    Angle y = toAngular<Length>(x, 2_cm);
    Length z = toLinear<Angle>(y, 2_cm);
    static_assert(Angle(5.1) >= Quantity<std::ratio<0>, std::ratio<0>, std::ratio<0>, std::ratio<0>, std::ratio<1>,
                                         std::ratio<0>, std::ratio<0>, std::ratio<0>>(5.0));
    units::max(10_celsius, Quantity<std::ratio<0>, std::ratio<0>, std::ratio<0>, std::ratio<0>, std::ratio<0>,
                                    std::ratio<1>, std::ratio<0>, std::ratio<0>>(1.0));
    // check Vector3D overloads
    units::Vector3D<Length> v3a = 2 * units::V3Position(2_in, 2_in, 2_in) * 2;
    units::Vector3D<Length> v3b = units::V3Position(2_in, 2_in, 2_in) / 2.0;
    units::Vector3D<Area> v3c = 2_in * units::V3Position(2_in, 2_in, 2_in);
    units::Vector3D<Area> v3d = units::V3Position(2_in, 2_in, 2_in) * 2_in;
    units::Vector3D<Number> v3e = units::V3Position(2_in, 2_in, 2_in) / 2_in;
    // check Vector2D overloads
    units::Vector2D<Length> v2a = units::V2Position(2_in, 2_in) / 2;
    units::Vector2D<Length> v2b = 2 * units::V2Position(2_in, 2_in) * 2;
    units::Vector2D<Area> v2c = 2_in * units::V2Position(2_in, 2_in);
    units::Vector2D<Area> v2d = units::V2Position(2_in, 2_in) * 2_in;
    units::Vector2D<Number> v2e = units::V2Position(2_in, 2_in) / 2_in;
}

constexpr void angleTests() {
    static_assert(+15_cDeg == 75_stDeg);
    static_assert(to_stDeg(-+15_cDeg) == to_stDeg(105_stDeg));
    static_assert(r2i(to_stDeg(30_cDeg)) == r2i(to_stDeg(60_stDeg)));
    static_assert(r2i(to_stDeg(+0_cDeg)) == r2i(to_stDeg(90_stDeg)));
    Angle a = 2_cDeg;
}

constexpr void angleRangeTests() {
    static_assert(r2i(to_stDeg(AngleRange(-15_cDeg))) == r2i(to_stDeg(AngleRange(+15_stDeg))));
    Angle a = 2_stDeg + AngleRange(15_stDeg);
    Angle b = AngleRange(15_stDeg) + 2_stDeg;
    Angle c = 2_stDeg + AngleRange(15_cDeg);
}

constexpr Number numAssignmentTests() {
    using namespace units_double_ops;
    Number n = 1_num; // 1
    n += 2; // 3
    n--; // 2
    n -= 3; // -1
    n *= 2; // -2
    n /= 2; // -1
    n++; // 0
    return n;
}

constexpr double doubleAssignmentTests() {
    using namespace units_double_ops;
    double d = 1; // 1
    d += 2_num; // 3
    d -= 2_num; // 1
    d *= 2_num; // 2
    d /= 2_num; // 1
    return d;
}

void numberOperatorTests() {
    using namespace units_double_ops;
    static_assert(1_num + 2 == 3);
    static_assert(1 + 2_num <= 3);
    static_assert(1 / 2_num >= 0);

    static_assert(numAssignmentTests() == 0);
    static_assert(doubleAssignmentTests() == 1);
}