#include "PhysicsSystem.h"


PhysicsSystem::PhysicsSystem() {
    
    world = nullptr;
    return;
}

void PhysicsSystem::Initiate(void) {
    
    rp3d::PhysicsWorld::WorldSettings worldSettings;
    worldSettings.defaultBounciness            = 0.5;
    worldSettings.defaultFrictionCoefficient   = 0.7;
    worldSettings.restitutionVelocityThreshold = 0.9;
    
    world = common.createPhysicsWorld(worldSettings);
    
    SetWorldGravity(0, -9.98 * 3 * 3, 0);
    
    world->enableSleeping(true);
    world->setSleepLinearVelocity(0.1);
    world->setSleepAngularVelocity(0.1);
    
    return;
}

void PhysicsSystem::SetWorldGravity(float x, float y, float z) {
    world->setGravity(rp3d::Vector3(x, y, z));
    return;
}


rp3d::RigidBody* PhysicsSystem::CreateRigidBody(float x, float y, float z) {
    
    rp3d::Vector3 position = rp3d::Vector3(x, y, z);
    rp3d::Quaternion orientation = rp3d::Quaternion::identity();
    rp3d::Transform physicsTransform = rp3d::Transform(position, orientation);
    
    rp3d::RigidBody* body = world->createRigidBody(physicsTransform);
    
    body->setMass(1);
    body->setLinearDamping(0.001);
    body->setAngularDamping(0.004);
    
    return body;
}

void PhysicsSystem::DestroyRigidBody(rp3d::RigidBody* rigidBodyPtr) {
    assert(rigidBodyPtr != nullptr);
    world->destroyRigidBody(rigidBodyPtr);
}


rp3d::BoxShape* PhysicsSystem::CreateColliderBox(float xscale, float yscale, float zscale) {
    rp3d::Vector3 scale(xscale, yscale, zscale);
    return common.createBoxShape(scale);
}

rp3d::SphereShape* PhysicsSystem::CreateColliderSphere(float radius) {
    return common.createSphereShape(radius);
}

rp3d::CapsuleShape* PhysicsSystem::CreateColliderCapsule(float radius, float height) {
    return common.createCapsuleShape(radius, height);
}


