#ifndef PARTICLE_HPP_INCLUDED
#define PARTICLE_HPP_INCLUDED

class Particle{
    PVector location;
    PVector velocity;
    PVector acceleration;
    float maxspeed;
    float maxforce;
    int mass;

    Shell shell;
    unsigned int target;

    void apply_force(PVector force, float dt){
        PVector f = mult(force, dt);
        acceleration = add(acceleration, f);
    }

    void update_speed(float dt){
        PVector a = mult(acceleration, dt);
        velocity = add(velocity, a);
        velocity = limit(velocity, maxspeed);
    }

    void move(float dt){
        PVector v = mult(velocity, dt);
        location = add(location, v);
    }
public:
    Particle(unsigned int energy_limit){
        shell.set_max(energy_limit);
        target = 10;
        shell.give(random(energy_limit));
        mass = 20;

        acceleration = PVector(0, 0);
        velocity = PVector(0, 0);
        location = PVector(0, 0);

        maxspeed = 50-mass;
        maxforce = 50;
    }

    int charge(){
        return target - shell.capacity();
        return shell.capacity() - target;
    }

    PVector position(){
        return location;
    }

    void set_position(PVector vertex){
        location = vertex;
    }

    void seek(PVector target, float force, float dt)
    {
        PVector desired = sub(target, location);
        desired = limit(desired, maxspeed);

        PVector steer = sub(desired, velocity);
        steer = mult(steer, force);
        steer = limit(steer, maxforce);
        apply_force(steer, dt);
    }

    void update(float dt)
    {
        update_speed(dt);
        move(dt);
    }

    float get_mass()
    {
        return mass;
    }

    float get_radius()
    {
        return mass/2;
    }

    Shell& get_shell()
    {
        return shell;
    }
};

#endif // PARTICLE_HPP_INCLUDED
