#ifndef PONG_UTIL_H
#define PONG_UTIL_H

namespace pong {

template<typename T>
inline const T clamp(const T& value, const T& min, const T& max) {
    return value < min ? min : (value > max ? max : value);
}

template<typename T>
inline const T abs(const T& x) {
    return (x < 0) ? -x : x;
}

} // namespace pong

#endif