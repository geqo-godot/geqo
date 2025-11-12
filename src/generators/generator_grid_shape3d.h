#pragma once
#include "query_generator3d.h"

namespace godot
{
    class CGeneratorGridShape3D : public CQueryGenerator3D
    {
        GDCLASS(CGeneratorGridShape3D, CQueryGenerator3D)

    protected:
        static void _bind_methods();

    public:
        CGeneratorGridShape3D();
        ~CGeneratorGridShape3D();

        void perform_generation(vector<CQueryItem>) override;

    private:
    };

}