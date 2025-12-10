#pragma once
#include <map>
#include <vector>
#include <algorithm>
#include <gdiplus.h>
#include "IRenderable.h"

class LayerController
{
public:

    struct LayerData
    {
        std::vector<IRenderable*> objects;
        Gdiplus::Bitmap* cache = nullptr;
        bool dirty = true;            
        bool hasDynamic = false;      
    };

    static void Add(IRenderable* obj, int layer)
    {
        LayerData& ld = layers()[layer];
        ld.objects.push_back(obj);

        if (obj->IsDynamic())
            ld.hasDynamic = true;

        ld.dirty = true;
    }

    static void Remove(IRenderable* obj)
    {
        for (auto& [layer, ld] : layers())
        {
            auto& vec = ld.objects;
            vec.erase(std::remove(vec.begin(), vec.end(), obj), vec.end());

            ld.hasDynamic = false;
            for (auto* o : vec)
                if (o->IsDynamic()) ld.hasDynamic = true;

            ld.dirty = true;
        }
    }

    static void MoveToLayer(IRenderable* obj, int newLayer)
    {
        Remove(obj);
        Add(obj, newLayer);
    }

    static void DrawAll(Gdiplus::Graphics& g)
    {
        auto& ls = layers();
        for (auto& [layer, ld] : ls)
        {
            if (ld.cache == nullptr)
            {
                ld.cache = new Gdiplus::Bitmap(2000, 2000, PixelFormat32bppARGB);
                ld.dirty = true;
            }

            if (ld.dirty || ld.hasDynamic)
            {
                Gdiplus::Graphics cacheG(ld.cache);
                cacheG.SetInterpolationMode(Gdiplus::InterpolationModeNearestNeighbor);
                cacheG.SetPixelOffsetMode(Gdiplus::PixelOffsetModeNone);
                cacheG.SetSmoothingMode(Gdiplus::SmoothingModeNone);
                cacheG.Clear(Gdiplus::Color(0, 0, 0, 0));

                for (auto* obj : ld.objects)
                    obj->Draw(cacheG);

                ld.dirty = false;
            }

            g.DrawImage(ld.cache, 0, 0);
        }
    }

private:
    static std::map<int, LayerData>& layers()
    {
        static std::map<int, LayerData> inst;
        return inst;
    }
};
