#ifndef TSERIES_H
#define TSERIES_H

#include <exception>
#include <vector>

namespace Pheux { namespace Core {

    template<class T>
    class TSeries
    {
    public:
        TSeries() {}
        ~TSeries() {}

        // the latest one always is [0]
        T operator[](int id)
        {
            return _data[mapId(id)];
        }

        T operator[](int id) const
        {
            return _data[mapId(id)];
        }

        int size() const { return _data.size(); }
        void add(T t) { _data.push_back(t); }
        void set(int id, T val) { _data[mapId(id)] = val; }
        void clear() { _data.clear(); }

    protected:
        vector<T> _data;

        inline int mapId(int id) const
        {
            if (!_data.empty() && id >= 0 && id < size())
            {
                return _data.size() - id - 1;
            }
            else
            {
                //throw exception("Invalid index.");
                return -1;
            }
        }
    };


}}


#endif
