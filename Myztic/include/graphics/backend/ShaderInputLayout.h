#pragma once

#include <graphics/backend/VAO.hpp>
#include <vector>

enum InputPropertyType {
    Position,
    Color,
    TextureCoordinate,
    Normal
};

struct InputProperty {
    InputPropertyType type;
    GLint size;
    GLenum glType;
    GLint pointerOffset;
};

struct LayoutDescription {
    std::vector<InputProperty> inputProperties;
    GLint elementCount;
};

class ShaderInputLayout {
public:
    ShaderInputLayout();
    static LayoutDescription createLayoutDescription(std::vector<InputProperty>& inputProperties);
    static ShaderInputLayout createInputLayout(LayoutDescription desc);
    inline void bindInputLayout() {
        attachedVAO.bind();
    };
    inline static void unbind() { VAO::unbind(); };
    void deleteInputLayout();
    void enableAttrib(int index);
    void enableAllAttribs();
    void disableAttrib(int index);
    void disableAllAttribs();

    int propertyCount;
    LayoutDescription description;
    VAO attachedVAO;
private:
    static InputProperty POSITION;
    static InputProperty COLOR;
    static InputProperty TEXCOORD;
    static InputProperty NORMAL;
};