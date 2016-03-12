#ifndef SHELL_HPP_INCLUDED
#define SHELL_HPP_INCLUDED

class Shell{
    std::vector<int> log;
    unsigned int energy;
    unsigned int size_limit;
public:
    Shell(){
        energy = 0;
        size_limit = 10;
    }

    void set_max(unsigned int max_energy){
        size_limit = max_energy;
    }

    Shell(unsigned int max_energy){
        energy = 0;
        set_max(max_energy);
    }

    unsigned int capacity(){
        return energy;
    }

    unsigned int available(){
        return size_limit - energy;
    }

    unsigned int give(unsigned int packs){
        int e= packs;
        if(e > available())
        {
            e = available();
        }
        energy += e;
        if(e != packs)
        {
            return packs - e;
        }
        else
        {
            return 0;
        }
    }

    unsigned int take(unsigned int packs)
    {
        int e = packs;
        if(e > energy)
        {
            e = energy;
        }
        energy -= e;
        return e;
    }

    std::vector<int>& history(){
        return log;
    }

};

#endif // SHELL_HPP_INCLUDED
