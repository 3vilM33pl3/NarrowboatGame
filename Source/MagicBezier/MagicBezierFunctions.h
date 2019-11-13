// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <vector>

/**
 * 
 */
class MAGICBEZIER_API MagicBezierFunctions
{
public:
	MagicBezierFunctions();
	~MagicBezierFunctions();

	static FVector CubicBezierPoint(const FVector P0, const FVector P1, const FVector P2, const FVector P3, const float T)
	{
		return pow(1.0 - T, 3) * P0 + 3 * pow(1.0 - T, 2) * T * P1 + 3*(1.0 - T)*pow(T,2)*P2 + pow(T, 3)*P3;
	}

	static std::vector<FVector> CubicBezierCurve(const FVector P0, const FVector P1, const FVector P2, const FVector P3, const float T)
	{
		std::vector<FVector> BezierCurvePoints;
		for (float i = 0; i <= 1; i=i+T)
		{
			FVector P = CubicBezierPoint(P0, P1, P2, P3, i);
			BezierCurvePoints.push_back(P);
		}
		return BezierCurvePoints;
	}

	static float CubicBezierCurveLength(std::vector<FVector> BezierCurvePoints)
	{
		float Length = 0;
		for(int i = 0; i < BezierCurvePoints.size() - 2; i++)
		{
			Length =+	FVector::Distance(BezierCurvePoints.at(i), BezierCurvePoints.at(i + 1));
		}
		return abs(Length);
	}
};
