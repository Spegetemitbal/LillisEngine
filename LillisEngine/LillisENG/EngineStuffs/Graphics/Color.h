#pragma once


class Color
{
private:
	int mRed, mGreen, mBlue, mAlpha;
public:
	Color() { mRed = mGreen = mBlue = mAlpha = 0; };
	Color(int r, int g, int b, int a) { mRed = r; mGreen = g; mBlue = b; mAlpha = a; };
	Color(int r, int g, int b) { mRed = r; mGreen = g; mBlue = b; mAlpha = 255; };
	~Color() {};
	int getR() { return mRed; };
	int getG() { return mGreen; };
	int getB() { return mBlue; };
	int getA() { return mAlpha; };
};