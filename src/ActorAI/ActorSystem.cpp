#include <GameEngineFramework/ActorAI/ActorSystem.h>
#include <GameEngineFramework/Logging/Logging.h>
#include <GameEngineFramework/Math/Random.h>

extern Logger Log;
extern ActorSystem AI;
extern NumberGeneration Random;
extern MathCore Math;

// Actor system thread
bool isActorThreadActive = true;
bool doUpdate = false;
void actorThreadMain(void);

int actorCounter=0;


ActorSystem::ActorSystem() : 
    playerPosition(0)
{
}

void ActorSystem::Initiate(void) {
    
    actorSystemThread = new std::thread( actorThreadMain );
    
    Log.Write( " >> Starting thread AI" );
    
    return;
}

void ActorSystem::Shutdown(void) {
    
    mux.lock();
    isActorThreadActive = false;
    mux.unlock();
    
    actorSystemThread->join();
    
    return;
}

void ActorSystem::SetPlayerWorldPosition(glm::vec3 position) {
    mux.lock();
    playerPosition = position;
    mux.unlock();
    return;
}

void ActorSystem::UpdateSendSignal(void) {
    mux.lock();
    doUpdate = true;
    mux.unlock();
    return;
}

Actor* ActorSystem::CreateActor(void) {
    mux.lock();
    Actor* newActor = mActors.Create();
    mux.unlock();
    return newActor;
}

bool ActorSystem::DestroyActor(Actor* actorPtr) {
    mux.lock();
    bool state = mActors.Destroy(actorPtr);
    mux.unlock();
    return state;
}

unsigned int ActorSystem::GetNumberOfActors(void) {
    mux.lock();
    unsigned int value = mActors.GetObjectCount();
    mux.unlock();
    return value;
}

Actor* ActorSystem::GetActor(unsigned int index) {
    mux.lock();
    Actor* actorPtr = mActors[index];
    mux.unlock();
    return actorPtr;
}


//
// AI actor processing
//

void ActorSystem::Update(void) {
    
    float force = 0.0001;
    
    glm::vec3 forward(0);
    
    unsigned int numberOfActors = (unsigned int)mActors.Size();
    
    mux.lock();
    
    for (int i = 0; i < 80; i++) {
        
        if (actorCounter >= numberOfActors) {
            actorCounter = 0;
            break;
        }
        
        Actor* actor = mActors[actorCounter];
        
        actorCounter++;
        
        actor->mux.lock();
        
        if (!actor->mIsActive) {
            actor->mux.unlock();
            continue;
        }
        
        // Advance actor age
        actor->mAge++;
        
        
        // Feed data through the network
        
        for (int a=1; a < actor->mWeightedLayers.size(); a++) {
            
            for (int w=0; w < NEURAL_LAYER_WIDTH; w++) 
                actor->mWeightedLayers[a].node[w] = actor->mWeightedLayers[a-1].node[w] * 
                                                    actor->mWeightedLayers[a-1].weight[w];
            
            continue;
        }
        
        
        // Apply neural plasticity
        // Plasticity is the amount to which the weights will conform to the data coming though
        
        for (int a=0; a < actor->mWeightedLayers.size(); a++) {
            
            for (int w=0; w < NEURAL_LAYER_WIDTH; w++) 
                actor->mWeightedLayers[a].weight[w] = Math.Lerp(actor->mWeightedLayers[a].weight[w], 
                                                                actor->mWeightedLayers[a].node[w], 
                                                                actor->mWeightedLayers[a].plasticity);
            
            continue;
        }
        
        // Apply forward velocity
        actor->mVelocity = forward;
        
        actor->mux.unlock();
        
        continue;
    }
    
    mux.unlock();
    
    return;
}



//
// Actor system thread
//

void actorThreadMain() {
    
    while (isActorThreadActive) {
        
        if (!doUpdate) {
            std::this_thread::sleep_for( std::chrono::duration<float, std::milli>(1) );
            continue;
        }
        
        AI.Update();
        
        doUpdate = false;
        continue;
    }
    
    std::this_thread::sleep_for( std::chrono::duration<float, std::milli>(1) );
    Log.Write( " >> Shutting down on thread AI" );
    
    return;
}

