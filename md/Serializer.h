#ifndef SERIALIZER_H
#define SERIALIZER_H

#include "Util.h"

namespace Pheux { namespace Core {

    class Tick;
    class BarSeries;
    class Bar;

    class Serializer
    {
    public:
        virtual void Serialize(Tick* tick) {}
        virtual void Deserialize(std::vector<Tick*>&) {}
        virtual void Deserialize(const std::string&, std::vector<Tick*>&) {}

        void Serialize(const void* sender, Tick*& arg)
        {
            Tick* tick = arg;
            Serialize(tick);
        }

        Serializer() {}
        virtual ~Serializer() {}
    };


    class TextSerializer : public Serializer
    {
    public:
        TextSerializer(const string& path, string delim = ",", int chunk = 1);
        virtual ~TextSerializer();

        virtual void Serialize(Tick* tick);
        virtual void Deserialize(std::vector<Tick*>& vec);
        virtual void Deserialize(const std::string&, std::vector<Tick*>&);

    private:
        std::string delim;
        int chunk;
        std::vector<Tick*> buffer;
        std::map<std::string, Tick*> buffer2;
        std::string path;

        string serTick(const Tick* tick) const;
        Tick* deserTick(const string& line);

        void dump();
    };

}}


#endif
