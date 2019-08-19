#include "Model.h"

uint32_t TextureFromFile(QOpenGLExtraFunctions* gl, const std::string& path, const std::string& directory)
{
     //Generate texture ID and load texture data
    std::string filename = directory + '/' + path;
    uint32_t textureID {0};
    QImage image;

    if (image.load(filename.c_str()))
    {
        gl->glGenTextures(1, &textureID);

        // Assign texture to ID
        gl->glBindTexture(GL_TEXTURE_2D, textureID);
        gl->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
        gl->glGenerateMipmap(GL_TEXTURE_2D);

        // Parameters
        gl->glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        gl->glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
        gl->glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
        gl->glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        gl->glBindTexture(GL_TEXTURE_2D, 0);
    }
    else {
        std::cout << "Failed to load texture " << filename << "!" << std::endl;
    }

    return textureID;
}
