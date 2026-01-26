#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryGenerator.h"
#include "DataProviders/AIDataProvider.h"
#include "EnvQueryGenerator_Searchables.generated.h"

class USearchPointComponent;

/**
 * Returns Actors "Only" with SearchPointComponent 
 */
UCLASS()
class ESCAPETHEOVERTIMEVR_API UEnvQueryGenerator_Searchables : public UEnvQueryGenerator
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = "Generator")
	FAIDataProviderFloatValue SearchRadius; // 탐색 반경

public:
	UEnvQueryGenerator_Searchables();

	virtual void GenerateItems(FEnvQueryInstance& QueryInstance) const override;
};
