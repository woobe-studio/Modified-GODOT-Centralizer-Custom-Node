/**************************************************************************/
/*  centralizer.cpp                                                           */
/**************************************************************************/

#include "centralizer.h"

#include "scene/main/viewport.h"

void Centralizer::_notification(int p_what) {
    switch (p_what) {
        case NOTIFICATION_ENTER_TREE: {
            set_position_preset(current_preset);
            Viewport *viewport = Object::cast_to<Viewport>(get_viewport());
            if (viewport) {
                Error err = viewport->connect("size_changed", this, "on_viewport_size_changed");
                if (err != OK) {
                    ERR_PRINT("Failed to connect signal 'size_changed in Centralizer'.");
                }
            } else {
                ERR_PRINT("Viewport is not available to Centralizer.");
            }
            break;
        }
        case NOTIFICATION_EXIT_TREE:
            Viewport *viewport = Object::cast_to<Viewport>(get_viewport());
            if (viewport) {
                viewport->disconnect("size_changed", this, "on_viewport_size_changed");
            }
            break;
    }
}

void Centralizer::update_position() {
    set_position_preset(current_preset);
}


void Centralizer::set_position_preset(LayoutPreset p_preset) {
    current_preset = p_preset;
    if(!get_viewport())
        return;

    Vector2 viewport_size = get_viewport()->get_visible_rect().size;
    Vector2 position;

    switch (p_preset) {
        case PRESET_TOP:
            position = Vector2(viewport_size.x * 0.5, 0);
            break;
        case PRESET_LEFT:
            position = Vector2(0, viewport_size.y * 0.5);
            break;
        case PRESET_RIGHT:
            position = Vector2(viewport_size.x, viewport_size.y * 0.5);
            break;
        case PRESET_BOTTOM:
            position = Vector2(viewport_size.x * 0.5, viewport_size.y);
            break;
        case PRESET_CENTER:
            position = Vector2(viewport_size.x * 0.5, viewport_size.y * 0.5);
            break;
        case PRESET_TOP_LEFT:
            position = Vector2(0, 0);
            break;
        case PRESET_TOP_RIGHT:
            position = Vector2(viewport_size.x, 0);
            break;
        case PRESET_BOTTOM_LEFT:
            position = Vector2(0, viewport_size.y);
            break;
        case PRESET_BOTTOM_RIGHT:
            position = Vector2(viewport_size.x, viewport_size.y);
            break;
    }

    set_global_position(position);
}

int Centralizer::get_position_preset() const {
    return current_preset;
}

void Centralizer::on_viewport_size_changed() {
    update_position();
}

void Centralizer::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_position_preset", "preset"), &Centralizer::set_position_preset);
    ClassDB::bind_method(D_METHOD("get_position_preset"), &Centralizer::get_position_preset);
    ClassDB::bind_method(D_METHOD("update_position"), &Centralizer::update_position);
    ClassDB::bind_method(D_METHOD("on_viewport_size_changed"), &Centralizer::on_viewport_size_changed);

    ADD_PROPERTY(PropertyInfo(Variant::INT, "preset", PROPERTY_HINT_ENUM, "Top, Left, Right, Center, Bottom, Top Left,Top Right,Bottom Left,Bottom Right"), "set_position_preset", "get_position_preset");
}

Centralizer::Centralizer() {
    current_preset = PRESET_CENTER;
}

Centralizer::~Centralizer() {
}