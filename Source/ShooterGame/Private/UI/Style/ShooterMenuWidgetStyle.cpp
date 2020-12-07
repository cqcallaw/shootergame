// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterMenuWidgetStyle.h"
#include "ShooterGame.h"

FShooterMenuStyle::FShooterMenuStyle()
{
}

FShooterMenuStyle::~FShooterMenuStyle()
{
}

const FName FShooterMenuStyle::TypeName(TEXT("FShooterMenuStyle"));

const FShooterMenuStyle& FShooterMenuStyle::GetDefault()
{
	static FShooterMenuStyle Default;
	return Default;
}

void FShooterMenuStyle::GetResources(TArray<const FSlateBrush*>& OutBrushes) const
{
	OutBrushes.Add(&HeaderBackgroundBrush);
	OutBrushes.Add(&LeftBackgroundBrush);
	OutBrushes.Add(&RightBackgroundBrush);
}


UShooterMenuWidgetStyle::UShooterMenuWidgetStyle( const FObjectInitializer& ObjectInitializer )
	: Super(ObjectInitializer)
{

}
