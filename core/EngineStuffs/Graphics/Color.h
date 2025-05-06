#pragma once


class Color
{
private:
	int mRed, mGreen, mBlue, mAlpha;
public:
	Color() { mRed = mGreen = mBlue = mAlpha = 0; };
	Color(int r, int g, int b, int a) { mRed = r; mGreen = g; mBlue = b; mAlpha = a; };
	Color(int r, int g, int b) { mRed = r; mGreen = g; mBlue = b; mAlpha = 255; };
	~Color() = default;
	int getR() const { return mRed; };
	int getG() const { return mGreen; };
	int getB() const { return mBlue; };
	int getA() const { return mAlpha; };
	float getRf() const { return (float)mRed / 255.0f; };
	float getGf() const { return (float)mGreen / 255.0f; };
	float getBf() const { return (float)mBlue / 255.0f; };
	float getAf() const { return (float)mAlpha / 255.0f; };
};