#pragma once
#include <core/log.h>
#include "math.h"

class ValueAction {
public:
	ValueAction() {}

	virtual void Interpolate(float percent) = 0;
	virtual float GetInterpolation() = 0;
};

template<typename T>
class ValueInterpolation : public ValueAction {
private:
	T* value;
	T minValue;
	T maxValue;
public:
	ValueInterpolation(T* value, T minValue, T maxValue) : value(value), minValue(minValue), maxValue(maxValue) {}

	void Interpolate(float percent) override { FD_WARNING("[ValueInterpolation] Function not implemented"); }
	float GetInterpolation() override { FD_WARNING("[ValueInterpolation] Function not implemented"); }

	inline T GetMin() const { return minValue; }
	inline T GetMax() const { return maxValue; }
	inline T* GetValue() const { return value; }

	inline void SetMin(T minValue) { this->minValue = minValue; }
	inline void SetMax(T maxValue) { this->maxValue = maxValue; }
	inline void SetValue(T* value) { this->value = value; }
};



template<>
void ValueInterpolation<float>::Interpolate(float percent) {
	*value = ((maxValue - minValue) * percent) + minValue;
}

template<>
float ValueInterpolation<float>::GetInterpolation() {
	return (*value - minValue) / (maxValue - minValue);
}

template<>
void ValueInterpolation<vec2>::Interpolate(float percent) {
	*value = ((maxValue - minValue) * percent) + minValue;
}

template<>
void ValueInterpolation<vec3>::Interpolate(float percent) {
	*value = ((maxValue - minValue) * percent) + minValue;
}

template<>
void ValueInterpolation<vec4>::Interpolate(float percent) {
	*value = ((maxValue - minValue) * percent) + minValue;
}
