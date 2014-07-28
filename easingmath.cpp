#include "easingmath.h"
#include "qmath.h"

EasingMath::EasingMath()
{
}

float EasingMath::toRadians(float angle)
{
  if (angle > 359) {
    int fangle = int(angle) / 360;
    angle -= fangle * 360;
  }

  return angle * (3.141592653 / 180);
}

float EasingMath::linearTween(float time, float startValue, float change, float duration) {
    return change * time / duration + startValue;
}
float EasingMath::easeInExponential(double x) {
    return pow(2, 10 * (x - 1));
}
float EasingMath::easeOutExponential(double x) {
    return -pow(2, -10 * x) + 1;
}
float EasingMath::easeInOutExponential(double x) {
    if (x < 0.5) return easeInExponential(x * 2) / 2; else return (easeOutExponential((x - 0.5) * 2) / 2) + 0.5;
}

float EasingMath::easeInCubic(double x) {
    return x * x * x;
}
float EasingMath::easeOutCubic(double x) {
    return 1 - ((1 - x) * (1 - x) * (1 - x));
}
float EasingMath::easeInOutCubic(double x) {
    if (x < 0.5) return easeInCubic(x * 2) / 2; else return (easeOutCubic((x - 0.5) * 2) / 2) + 0.5;
}
float EasingMath::easeInQuadratic(double x) {
    return x * x;
}
float EasingMath::easeOutQuadratic(double x) {
    return 1 - ((1 - x) * (1 - x));
}
float EasingMath::easeInOutQuadratic(double x) {
    if (x < 0.5) return easeInQuadratic(x * 2) / 2; else return (easeOutQuadratic((x - 0.5) * 2) / 2) + 0.5;
}
float EasingMath::easeInSinusoidal(double x) {
    return sin(x *(M_PI / 2));
}
float EasingMath::easeOutSinusoidal(double x) {
    return 1 - cos(x *(M_PI / 2));
}
float EasingMath::easeInOutSinusoidal(double x) {
    return (sin(x * (M_PI) - (M_PI / 2)) + 1) / 2;
}
