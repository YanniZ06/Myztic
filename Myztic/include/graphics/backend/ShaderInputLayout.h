#pragma once

#include <graphics/backend/VAO.hpp>
#include <vector>

namespace Myztic {
    enum InputPropertyType {
        Position,
        Color,
        TextureCoordinate,
        Normal
    };

    struct InputProperty {
        //semantic type
        InputPropertyType type;
        //as in how many elements this InputProperty
        GLint size;
        //as in GL_FLOAT for example
        GLenum glType;
        //calculated automatically
        GLint pointerOffset;
        //aka: sizeof(GLfloat), for example
        unsigned long long sizeOfType;
    };

    struct LayoutDescription {
        std::vector<InputProperty> inputProperties;
        GLint elementCount;
    };

    class ShaderInputLayout {
        friend class Drawable;
    public:
        ShaderInputLayout(LayoutDescription description);
        ShaderInputLayout() = default;

        /**
         * Create input layout description which is passed into createInputLayout
         * \param inputProperties Array of typedef InputProperty which consists of `type:InputPropertyType` `size:Int` `glType:Int` `pointerOffset:Int`, pointerOffset is automatically calculated
         * \return LayoutDescription
         */
        static LayoutDescription createLayoutDescription(std::vector<InputProperty>& inputProperties);

        void setVertexLayout();
        inline void bindInputLayout() {
            attachedVAO.bind();
        };
        inline static void unbind() { VAO::unbind(); };
        void deleteInputLayout();
        /**
         * Enables a specific attribute
         * \param index Enables that attribute by index (You should know which index it is from the InputProperties you passed in)
         */
        void enableAttrib(int index);
        /**
         * Enables all of our input layout attributes
         */
        void enableAllAttribs();
        /**
         * Disables a specific attribute
         *
         * \param index Disables that attribute by index (You should know which index it is from the InputProperties you passed in)
         */
        void disableAttrib(int index);
        /**
         * Disables all of our input layout attributes.
         *
         */
        void disableAllAttribs();
        int propertyCount;

        static InputProperty POSITION;
        static InputProperty COLOR;
        static InputProperty TEXCOORD;
        static InputProperty NORMAL;
        LayoutDescription description;
    private:

        VAO attachedVAO;
    };
}