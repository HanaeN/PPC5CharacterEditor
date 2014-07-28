#ifndef EASINGMATH_H
#define EASINGMATH_H
#include <QtCore/qglobal.h>
class EasingMath
{
public:
    EasingMath();
    template <typename T>
    static double ease(double x, double xMin, double xMax,
                       double yMin, double yMax, T func) {
        x = (x - xMin)/(xMax - xMin);
        return (func(x) * (yMax - yMin)) + yMin;
    }
    template <double (*in)(double), double (*out)(double)>
    static double easeInOut(double x) { // use as ease(0, 0, 0, 0, 0, easeInOut<func, func>);
        if (x < 0.5) return in(x * 2) / 2; else return (out((x - 0.5) * 2) / 2) + 0.5;
    }
    static float toRadians(float angle);
    static float linearTween(float time, float startValue, float change, float duration);
    static float easeInExponential(double x);
    static float easeOutExponential(double x);
    static float easeInOutExponential(double x);
    static float easeInQuadratic(double x);
    static float easeOutQuadratic(double x);
    static float easeInOutQuadratic(double x);
    static float easeInCubic(double x);
    static float easeOutCubic(double x);
    static float easeInOutCubic(double x);
    static float easeInSinusoidal(double x);
    static float easeOutSinusoidal(double x);
    static float easeInOutSinusoidal(double x);
};

#endif // EASINGMATH_H
