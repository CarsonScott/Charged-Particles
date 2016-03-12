#ifndef PARTICLESYSTEM_HPP_INCLUDED
#define PARTICLESYSTEM_HPP_INCLUDED

#include "Shell.hpp"
#include "Particle.hpp"
#include "ParticleTracker.hpp"

class ParticleSystem{
    std::vector<Particle> particles;
    std::vector<sf::CircleShape> visual;
    std::vector<sf::Text> numbers;
    sf::Font* font;
    unsigned int limit;
    int net_charge;

    float force(float m1, float m2, float d)
    {
        float f = m1*m2 / pow(d,2);
        return f;
    }

public:
    ParticleSystem(unsigned int size_limit, sf::Font& f){
        net_charge = 0;
        font = &f;
        limit = size_limit;
        for(int i = 0; i < limit; i++){
            unsigned int p_energy = 20;
            Particle particle = Particle(p_energy);
            particles.push_back(particle);

            float radius = particle.get_radius()*2;
            sf::CircleShape circle(radius);

            int c = particle.charge();
            circle.setFillColor(sf::Color(255-c*255/p_energy, 255-c*255/p_energy, c*255/p_energy, 150));
            circle.setOutlineColor(sf::Color(255, 255, 255));
            circle.setOutlineThickness(3);
            circle.setOrigin(radius, radius);
            visual.push_back(circle);

            sf::Text t;
            t.setFont(*font);
            t.setCharacterSize(radius);
            t.setColor(sf::Color(255, 255, 255));
            numbers.push_back(t);
        }
    }

    void disperse(PVector window_size){
        for(int i = 0; i < particles.size(); i++){
            PVector position(random(window_size.x), random(window_size.y));
            particles[i].set_position(position);
        }
    }

    void update(float dt){
        int net_c = 0;
        for(int i = 0; i < particles.size(); i++){
            int c = particles[i].charge();
            net_c += c;

            for(int j = 0; j < particles.size(); j++){
                if(i != j){
                    PVector pos1 = particles[i].position();
                    PVector pos2 = particles[j].position();
                    float dist = getDistance(pos1, pos2);

                    float charge1 = particles[i].charge();
                    float charge2 = particles[j].charge();
                    float f = -force(charge1, charge2, dist);

                    float mass1 = particles[i].get_mass();
                    float mass2 = particles[j].get_mass();
                    float g = force(mass1, mass2, dist);

                    float min_distance = particles[i].get_radius() + particles[j].get_radius();

                    PVector target(pos2.x, pos2.y);
                    if(dist < 800)
                    {
                        if(dist > min_distance)
                        {
                            particles[i].seek(target, f, dt);
                            particles[i].seek(target, g, dt);
                        }
                        else
                        {
                            particles[i].seek(target, f, dt);
                            particles[i].seek(target, -pow(g, 2), dt);

                            int av1 = particles[i].get_shell().available();
                            int av2 = particles[j].get_shell().available();

                            int cap1 = particles[i].get_shell().capacity();
                            int cap2 = particles[j].get_shell().capacity();

                            if(f >= 0)
                            {
                                int net = 1;
                                if(charge1 > 0)
                                {
                                    int e = particles[j].get_shell().take(net);
                                    int r = particles[i].get_shell().give(e);
                                    particles[j].get_shell().give(r);
                                }
                                else
                                {
                                    int e = particles[i].get_shell().take(net);
                                    int r = particles[j].get_shell().give(e);
                                    particles[i].get_shell().give(r);
                                }
                            }
                        }
                    }
                }
            }
            particles[i].update(dt);
            numbers[i].setString(to_string(particles[i].charge()));
            numbers[i].setOrigin(numbers[i].getGlobalBounds().width/2, numbers[i].getGlobalBounds().height/2);
            numbers[i].setPosition(particles[i].position());

            visual[i].setPosition(particles[i].position());
        }

        if(net_c != net_charge)
            std::cout << net_c << "\n";

        net_charge = net_c;
    }

    void draw(sf::RenderWindow& window)
    {
        for(int i = 0; i < visual.size(); i++)
        {
            window.draw(visual[i]);
            window.draw(numbers[i]);
        }
    }

    std::vector<Particle>& get_particles()
    {
        return particles;
    }

    void erase(unsigned int index)
    {
        particles.erase(particles.begin() + index);
        numbers.erase(numbers.begin() + index);
        visual.erase(visual.begin() + index);
    }

    void clear()
    {
        particles.clear();
        numbers.clear();
        visual.clear();
    }
};

#endif // PARTICLESYSTEM_HPP_INCLUDED
