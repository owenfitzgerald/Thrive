// Copyright (C) 2013-2017  Revolutionary Games

#include "Thrive.h"
#include "CellBase.h"

// Sets default values
ACellBase::ACellBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this
 	// off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


    // BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
    // RootComponent = BoxComponent;
    // BoxComponent->InitBoxExtent(FVector(50, 50, 50));
    // BoxComponent->SetCollisionProfileName(TEXT("Pawn"));
    //BoxComponent->SetCollisionProfileName(TEXT("BlockAll"));

    // USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(
    //     TEXT("RootComponent"));
    // RootComponent = SphereComponent;
    // SphereComponent->InitSphereRadius(40.0f);
    // SphereComponent->SetCollisionProfileName(TEXT("Pawn"));
    

    //MembraneComponent = CreateDefaultSubobject<UMembraneComponent>(TEXT("RootMembrane"));
    
    RuntimeMesh = CreateDefaultSubobject<URuntimeMeshComponent>(TEXT("Runtime Mesh"));
    RootComponent = RuntimeMesh;
    
    //MembraneComponent->SetupAttachment(RootComponent);
    //RootComponent = MembraneComponent;

    RuntimeMesh->SetSimulatePhysics(true);
    RuntimeMesh->bUseComplexAsSimpleCollision = false;
    
    //SetSimulatePhysics(true);
    //MembraneComponent->WakeRigidBody();
    
    //MembraneComponent->InitSphereRadius(40.0f);
    //MembraneComponent->SetCollisionProfileName(TEXT("Pawn"));

    
    
    OurMovementComponent = CreateDefaultSubobject<UCellPawnMovementComponent>(
         TEXT("CustomMovementComponent"));
    OurMovementComponent->UpdatedComponent = RootComponent;
    OurMovementComponent->PushComponent = RuntimeMesh;

    //OurMovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(
    //    TEXT("Movement"));
    //OurMovementComponent->UpdatedComponent = RootComponent;

    // This prevents the collision mesh to fall on z axis which would break the collision
    OurMovementComponent->SetPlaneConstraintEnabled(true);
    OurMovementComponent->SetPlaneConstraintNormal(FVector(0, 0, 1));
    


    //SoundVisComponent = CreateDefaultSubobject<USoundVisComponent>(TEXT("TestMusic"));
    
    
    // UStaticMeshComponent* SphereVisual = CreateDefaultSubobject<UStaticMeshComponent>(
    //     TEXT("VisualRepresentation"));
    
    // SphereVisual->SetupAttachment(RootComponent);
    
    // static ConstructorHelpers::FObjectFinder<UStaticMesh>
    //     SphereVisualAsset(TEXT("/Engine/BasicShapes/Sphere"));
    
    // if(SphereVisualAsset.Succeeded()){
        
    //     SphereVisual->SetStaticMesh(SphereVisualAsset.Object);
    //     //SphereVisual->SetMaterial(0, class UMaterialInterface *Material)
    //     SphereVisual->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
    //     SphereVisual->SetWorldScale3D(FVector(0.8f));
    // }


    //AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
    //AudioComponent->SetupAttachment(RootComponent);
    
}

// Called when the game starts or when spawned
void ACellBase::BeginPlay()
{
	Super::BeginPlay();

    

    // if(VacuoleClass){

    //     Vacuole = NewObject<UOrganelleComponent>(this, *VacuoleClass);
    //     AddInstanceComponent(Vacuole);

    //     if(GEngine)
    //         GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Blue,
    //             TEXT("Vacuole spawned"));
    // }
}

void ACellBase::OnConstruction(const FTransform& Transform)
{
    RuntimeMesh->bUseComplexAsSimpleCollision = false;
    
	TArray<FVector> Vertices;
	TArray<FVector> Normals;
	TArray<FRuntimeMeshTangent> Tangents;
	TArray<FVector2D> TextureCoordinates;
	TArray<int32> Triangles;

	URuntimeMeshLibrary::CreateBoxMesh(FVector(50, 50, 50), Vertices, Triangles, Normals,
        TextureCoordinates, Tangents);



	// Create the mesh section specifying collision
	RuntimeMesh->CreateMeshSection(0, Vertices, Triangles, Normals, TextureCoordinates,
        TArray<FColor>(), Tangents, true, EUpdateFrequency::Infrequent);

    RuntimeMesh->AddCollisionConvexMesh(Vertices);

    if(RuntimeMesh && RuntimeMesh->GetBodyInstance())
    {
        // RuntimeMesh->GetBodyInstance()->bLockZTranslation = true;
        
        RuntimeMesh->GetBodyInstance()->bLockXRotation = true;
        RuntimeMesh->GetBodyInstance()->bLockYRotation = true;

        RuntimeMesh->GetBodyInstance()->SetDOFLock(EDOFMode::XYPlane);
    }
}


// Called every frame
void ACellBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACellBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAction("ToggleEngulf", IE_Pressed, this,
        &ACellBase::ToggleEngulf);
    

    
    PlayerInputComponent->BindAxis("MoveForward", this, &ACellBase::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ACellBase::MoveRight);
}

// ------------------------------------ //
// Movement code

UPawnMovementComponent* ACellBase::GetMovementComponent() const
{
    return OurMovementComponent;
}

void ACellBase::MoveForward(float AxisValue){

    if(OurMovementComponent
        && (OurMovementComponent->UpdatedComponent == RootComponent)
    ){
        // This GetActorForwardVector() call makes the movement
        // relative to the cell heading
        OurMovementComponent->AddInputVector(GetActorForwardVector() * AxisValue);
    }
}

void ACellBase::MoveRight(float AxisValue){

    if(OurMovementComponent
        && (OurMovementComponent->UpdatedComponent == RootComponent)
    ){
        OurMovementComponent->AddInputVector(GetActorRightVector() * AxisValue);
    }
}

void ACellBase::ToggleEngulf(){

    if(GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Blue,
            TEXT("Toggle Engulf"));
    
}

