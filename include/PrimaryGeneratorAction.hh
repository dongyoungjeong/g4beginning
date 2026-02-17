#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h

#include "G4VUserPrimaryGeneratorAction.hh"

class G4ParticleGun;
class G4Event;
class G4Box;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
    PrimaryGeneratorAction();
    ~PrimaryGeneratorAction() override;

    void GeneratePrimaries(G4Event*) override; // method from the base class
    const G4ParticleGun* GetParticleGun() const { return fParticleGun; } // method to access particle gun

private:
    G4ParticleGun* fParticleGun = nullptr;  // pointer a to G4 gun class
    G4Box* fEnvelopeBox = nullptr;
};

#endif
