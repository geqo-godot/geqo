#pragma once
#include "query_generator3d.h"
#include "query_context3d.h"
#include <vector>
using namespace godot;
class CGeneratorGridShape3D : public CQueryGenerator3D
{
    GDCLASS(CGeneratorGridShape3D, CQueryGenerator3D)

protected:
    static void _bind_methods();

public:
    CGeneratorGridShape3D();
    ~CGeneratorGridShape3D();

    double get_grid_half_size() const;
    void set_grid_half_size(double size);

    double get_space_between() const;
    void set_space_between(double space);

    CQueryContext3D *get_generate_around();
    void set_generate_around(CQueryContext3D *context);

    bool get_use_vertical_projection() const;
    void set_use_vertical_projection(bool use);

    double get_project_down() const;
    void set_project_down(double project);

    double get_project_up() const;
    void set_project_up(double project);

    double get_post_projection_vertical_offset() const;
    void set_post_projection_vertical_offset(double offset);

    int get_projection_collision_mask() const;
    void set_projection_collision_mask(int mask);

    void perform_generation(std::vector<CQueryItem> &query_item_list) override;

private:
    // QueryGenerator
    double grid_half_size = 20.0;
    double space_between = 5.0;
    CQueryContext3D *generate_around = nullptr;

    // Projection Data
    bool use_vertical_projection = true;
    double project_down = 100.0;
    double project_up = 100.0;
    double post_projection_vertical_offset = 0.0;
    int projection_collision_mask = 1;
};