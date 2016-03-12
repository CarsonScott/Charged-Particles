#ifndef PARTICLETRACKER_HPP_INCLUDED
#define PARTICLETRACKER_HPP_INCLUDED

class ParticleTracker
{
    Particle* particle;
    sf::Color color;
    sf::VertexArray positions;
    PVector last_pos;
    float total_distance;
    float timer;
    float limit;
    float elapsed;
public:

    ParticleTracker(Particle* p, sf::Color c)
    {
        last_pos = PVector(0, 0);
        particle = p;
        color = c;
        total_distance = 0;

        timer = 0;
        limit = .1;

        positions.setPrimitiveType(sf::PrimitiveType::Points);
    }

    void update(float dt)
    {
        timer += dt;
        if(timer >= limit)
        {
            timer = 0;
            sf::Vertex point(particle->position(), color);
            if(positions.getVertexCount() > 0)
            {
                total_distance += getDistance(point.position, last_pos);

            }

            positions.append(point);
            last_pos = point.position;
        }
    }

    void draw(sf::RenderWindow& window)
    {
        window.draw(positions);
    }

    float get_distance()
    {
        return total_distance;
    }

    float avg_speed()
    {
        std::vector<float> speeds;
        float total = 0;
        for(int i = 1; i < positions.getVertexCount(); i++)
        {
            PVector current = positions[i].position;
            PVector last = positions[i-1].position;
            float d = getDistance(current, last);
            total += d;
        }

        return total / positions.getVertexCount();

        float sum = 0;
        for(int i = 0; i < speeds.size(); i++)
        {
            sum += speeds[i];
        }
        float avg = sum / speeds.size();
        return avg;
    }
};

#endif // PARTICLETRACKER_HPP_INCLUDED
