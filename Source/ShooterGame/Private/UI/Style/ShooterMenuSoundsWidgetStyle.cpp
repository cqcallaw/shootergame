// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterMenuSoundsWidgetStyle.h"
#include "ShooterGame.h"

FShooterMenuSoundsStyle::FShooterMenuSoundsStyle()
{
}

FShooterMenuSoundsStyle::~FShooterMenuSoundsStyle()
{
}

const FName FShooterMenuSoundsStyle::TypeName(TEXT("FShooterMenuSoundsStyle"));

const FShooterMenuSoundsStyle& FShooterMenuSoundsStyle::GetDefault()
{
	static FShooterMenuSoundsStyle Default;
	return Default;
}

void FShooterMenuSoundsStyle::GetResources(TArray<const FSlateBrush*>& OutBrushes) const
{
}


UShooterMenuSoundsWidgetStyle::UShooterMenuSoundsWidgetStyle( const FObjectInitializer& ObjectInitializer )
	: Super(ObjectInitializer)
{

}
