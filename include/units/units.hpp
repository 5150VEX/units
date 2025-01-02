#pragma once

#include <numeric>
#include <type_traits>

struct Rational {
    const int numerator;
    const int denominator;

    constexpr Rational(
        int numerator, 
        int denominator
    ) : 
        numerator(numerator / std::gcd(numerator, denominator)),
        denominator(denominator / std::gcd(numerator, denominator)) {
    
    }

    constexpr Rational(int numerator) : numerator(numerator), denominator(1) {

    }

    constexpr Rational operator+(const Rational& other) const {
        return Rational(
            numerator * other.denominator + other.numerator * denominator,
            other.denominator * other.denominator
        );
    }

    constexpr Rational operator-(const Rational& other) const {
        return Rational(
            numerator * other.denominator - other.numerator * denominator,
            denominator * other.denominator
        );
    }

    constexpr Rational operator*(const Rational& other) const {
        return Rational(
            numerator * other.numerator,
            denominator * other.denominator
        );
    }

    constexpr Rational operator/(const Rational& other) const {
        return Rational(
            numerator * other.denominator,
            denominator * other.numerator
        );
    }
};

template<typename D>
concept IsDimensionsT = requires(D a, D k, Rational j) {
    { a + k } -> std::same_as<D>;
    { a - k } -> std::same_as<D>;
    { a * j } -> std::same_as<D>;
    { a / j } -> std::same_as<D>;
};

template<auto D>
concept IsDimensions = IsDimensionsT<decltype(D)>;

struct SI {
    const Rational length = Rational(0);
    const Rational time = Rational(0);

    constexpr inline auto operator+(const SI& other) const {
        return SI {.length = length + other.length, .time = time + other.time };
    }

    constexpr inline auto operator-(const SI& other) const {
        return SI {.length = length - other.length, .time = time - other.time };
    }

    constexpr inline auto operator*(const Rational& other) const {
        return SI {.length = length * other, .time = time * other };
    }

    constexpr inline auto operator/(const Rational& other) const {
        return SI {.length = length / other, .time = time / other };
    }
};

template<auto D, typename R = double> requires IsDimensions<D>
class Quantity {
    R value;
public:
    template<auto E, typename S> requires IsDimensions<E>
    friend class Quantity;

    explicit constexpr inline Quantity<D, R>(R value) : value(value) {};

    constexpr inline auto operator+(const Quantity<D, R>& other) const {
        return Quantity<D, R>(value + other.value);
    };

    constexpr inline auto operator-(const Quantity<D, R>& other) const {
        return Quantity<D, R>(value - other.value);
    };

    template<auto E> requires IsDimensions<E>
    constexpr inline auto operator*(const Quantity<E, R>& other) {
        return Quantity<D + E, R>(value * other.value);
    }

    template<auto E> requires IsDimensions<E>
    constexpr inline auto operator/(const Quantity<E, R>& other) {
        return Quantity<D - E, R>(value / other.value);
    }
};