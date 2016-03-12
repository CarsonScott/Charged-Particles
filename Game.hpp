#ifndef GAME_HPP_INCLUDED
#define GAME_HPP_INCLUDED

#include <iostream>
#include <SFML/Graphics.hpp>
#include <ctime>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <string>
#include <sstream>
#include "Toolkit.hpp"
#include "ParticleSystem.hpp"

class Game{
    AdjustableView view;

    sf::Vector2f mousePos(sf::RenderWindow& screen){
        sf::Vector2f mouse = sf::Vector2f(sf::Mouse::getPosition(screen));
        return mouse;
    }

    sf::Vector2f windowSize(sf::RenderWindow& screen){
        sf::Vector2f winSize = sf::Vector2f(screen.getSize());
        return winSize;
    }

public:
    void main(sf::RenderWindow& window){
        view.setSize(windowSize(window));
        view.setCenter(PVector(window.getSize().x/2, window.getSize().y/2));
        window.setView(view);

        float time = 0;
        float time_limit = 1;

        sf::Font font;
        font.loadFromFile("consola.ttf");

        unsigned int particles = 75;
        ParticleSystem ps(particles, font);
        ps.disperse(windowSize(window));

        std::vector<ParticleTracker> trackers;
        for(int i = 0; i < particles; i++)
        {
            sf::Color color(150, 50, 15);
            color = sf::Color::White;
            ParticleTracker pt = ParticleTracker(&ps.get_particles()[i], color);
            trackers.push_back(pt);
        }

        sf::Text distance;
        distance.setFont(font);

        std::vector<Button*> buttons;

        Button new_particle;
        new_particle.setFont(font);
        new_particle.setPosition(700, 0);
        new_particle.add_str("New Particle");
        buttons.push_back(&new_particle);

        Button show_p;
        show_p.setFont(font);
        show_p.setPosition(15, 0);
        show_p.add_str("Hide Particles");
        show_p.add_str("Show Particles");
        buttons.push_back(&show_p);
        bool disp_particles = true;

        Button show_t(true);
        show_t.setFont(font);
        show_t.setPosition(15, 50);
        show_t.add_str("Hide Trackers");
        show_t.add_str("Show Trackers");
        buttons.push_back(&show_t);
        bool disp_trackers = true;

        Button show_n(true);
        show_n.setFont(font);
        show_n.setPosition(15, 100);
        show_n.add_str("Hide Net");
        show_n.add_str("Show Net");
        buttons.push_back(&show_n);
        bool disp_net = true;

        sf::VertexArray net(sf::PrimitiveType::LinesStrip, particles);
        for(int i = 0; i < particles; i++)
        {
            sf::Color c(255, 255, 255);
            net[i].color = c;
        }

        std::vector<std::string> ordered_values;

        sf::Clock clock;
        while (window.isOpen()){
            float dt = clock.restart().asSeconds();

            time += dt;
            if(time >= time_limit){
                time = 0;
            }

            sf::Event event;
            while (window.pollEvent(event)){
                view.handleEvent(event);

                for(int i = 0; i < buttons.size(); i++)
                {
                    buttons[i]->handleEvent(event, mousePos(window));
                }

                if (event.type == sf::Event::Closed)
                {
                    window.close();
                    std::ofstream record;
                    record.open("log.txt");

                    for(int i = 0; i < ordered_values.size(); i++)
                    {
                        record << ordered_values[i] << " ";
                        if(i % 25 == 0)
                        {
                            record << "\n";
                        }
                    }
                    record.close();
                }
                else if(event.type == sf::Event::KeyPressed)
                {
                    if(event.key.code == sf::Keyboard::C)
                    {
                        ps.clear();
                        for(int i = 0; i < trackers.size(); i++)
                        {
                            trackers.clear();
                        }
                        net.clear();
                    }

                    else if(event.key.code == sf::Keyboard::V)
                    {
                        std::vector<int> history;
                        for(int i = 0; i < ps.get_particles().size(); i++)
                        {
                            history = ps.get_particles()[i].get_shell().history();
                            for(int j = 0; j < history.size(); j++)
                            {
                                std::cout << history[j] << "\n";
                            }
                        }
                    }
                }
            }

            window.clear();
            window.setView(view);

            if(show_p.selected()){
                if(disp_particles)
                {
                    disp_particles = false;
                }
                else
                {
                    disp_particles = true;
                }
            }

            if(show_t.selected()){
                if(disp_trackers)
                {
                    disp_trackers = false;
                }
                else
                {
                    disp_trackers = true;
                }
            }
            if(show_n.selected())
            {
                if(disp_net)
                {
                    disp_net = false;
                }
                else
                {
                    disp_net = true;
                }
            }

            float dist = 0;
            for(int i = 0; i < ps.get_particles().size(); i++)
            {
                PVector pos(ps.get_particles()[i].position());
                net[i].position = pos;

                trackers[i].update(dt);
                if(disp_trackers){
                    trackers[i].draw(window);
                }
            }
            if(disp_net)
            {
                window.draw(net);
                if(net.getVertexCount() != ps.get_particles().size())
                {
                    net.resize(ps.get_particles().size());
                }
            }

            ps.update(dt);
            if(disp_particles){
                ps.draw(window);
            }

            window.setView(window.getDefaultView());
            for(int i = 0; i < buttons.size(); i++)
            {
                buttons[i]->update();
                window.draw(*buttons[i]);
            }
            window.display();
        }

    }
};

#endif // GAME_HPP_INCLUDED
