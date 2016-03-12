#define PVector sf::Vector2f
#define PI 3.1415926

int random(double limit, int base=0)
{
    int r = rand() % int(limit) + base;
    if(r >= limit)
    {
        r = limit;
    }
    return r;
}

std::string random_str(int length)
{
    std::string alphabet = "abcdefghijklmnopqrstuvwxyz";
    std::string str = "";
    for(int i = 0; i < length; i++)
    {
        str += alphabet.at(random(26));
    }
    return str;
}

std::string to_string(float number)
{
    std::stringstream ss(std::stringstream::in | std::stringstream::out);
    ss << number;
    return ss.str();
}

PVector getOffset(PVector a, PVector b)
{
    PVector offset(b.x - a.x, b.y - a.y);
    return offset;
}

float getDistance(PVector a, PVector b)
{
    PVector offset = getOffset(a, b);
    float distance = sqrt(pow(offset.x, 2) + pow(offset.y, 2));
    return distance;
}

/*---------------------------------------------------*/

class AdjustableView: public sf::View
{
    sf::RenderWindow* window;
public:
    void setWindow(sf::RenderWindow* w){
        window = w;
    }
    void handleEvent(sf::Event& event){
        if(event.type == sf::Event::KeyPressed)
        {
            if(event.key.code == sf::Keyboard::S){
                setSize(getSize().x * 1.25, getSize().y * 1.25);
            }
            if(event.key.code == sf::Keyboard::W){
                setSize(getSize().x / 1.25, getSize().y / 1.25);
            }

        }
    }
};

/*---------------------------------------------------*/

class Button: public sf::Text
{
    bool active;
    std::vector<std::string> strings;
    unsigned int index;
public:
    Button(bool initial_state=false)
    {
        active = initial_state;
        index = 0;
    }

    void add_str(std::string str)
    {
        strings.push_back(str);
        setString(strings[index]);
    }

    void handleEvent(sf::Event& event, PVector mousePos)
    {
        if(event.type == sf::Event::MouseButtonPressed)
        {
            if(getGlobalBounds().contains(mousePos))
            {
                active = true;
            }
        }
    }

    bool selected()
    {
        return active;
    }

    void update()
    {
        if(active)
        {
            active = false;
            index ++;
            if(index >= strings.size())
            {
                index = 0;
            }

            setString(strings[index]);
        }
    }
};

/*---------------------------------------------------*/

class Line: public sf::RectangleShape
{
public:
    void setPoints(PVector a, PVector b)
    {
       float d = getDistance(a, b);

       setSize(PVector(d, 2));
       setPosition(a);

       PVector offset = getOffset(a, b);
       float angle = atan2(offset.y, offset.x);
       setRotation(angle*180/PI);
    }
};

/*---------------------------------------------------*/

int normalize(float n)
{
    if(n != 0)
    {
        n = n/abs(n);
    }
    return n;
}

PVector normalize(PVector v)
{
    PVector p;
    if(v.x != 0)
    {
        p.x = v.x / abs(v.x);
    }
    if(v.y != 0)
    {
        p.y = v.y / abs(v.y);
    }
    return p;
}

PVector div(PVector v, float n)
{
    PVector p;
    p.x = v.x/n;
    p.y = v.y/n;
    return p;
}

PVector mult(PVector v, float n)
{
    PVector p;
    p.x = v.x*n;
    p.y = v.y*n;
    return p;
}

PVector sub(PVector a, PVector b)
{
    PVector d;
    d.x = a.x - b.x;
    d.y = a.y - b.y;
    return d;
}

PVector add(PVector a, PVector b)
{
    PVector s;
    s.x = a.x + b.x;
    s.y = a.y + b.y;
    return s;
}

PVector add(PVector v, float n)
{
    PVector p(v.x*n, v.y*n);
    return p;
}

PVector limit(PVector v, float l)
{
    PVector p = v;
    if(abs(v.x) > l)
    {
        p.x = l * v.x/abs(v.x);
    }
    if(abs(v.y) > l)
    {
        p.y = l * v.y/abs(v.y);
    }
    return p;
}

PVector flip(PVector v)
{
    PVector p;
    p.x = v.y;
    p.y = v.x;
    return p;
}
