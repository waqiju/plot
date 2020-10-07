#pragma once

class Texture2D
{
public:
    // holds the ID of the texture object, used for all texture operations to reference to this particlar texture
    unsigned int id;
    // texture image dimensions
    unsigned int width, height; // width and height of loaded image in pixels
    // texture Format
    unsigned int internalFormat; // format of texture object
    unsigned int imageFormat; // format of loaded image
    // texture configuration
    unsigned int wrapS; // wrapping mode on S axis
    unsigned int wrapT; // wrapping mode on T axis
    unsigned int filterMin; // filtering mode if texture pixels < screen pixels
    unsigned int filterMax; // filtering mode if texture pixels > screen pixels
    // constructor (sets default texture modes)
    Texture2D();
    // generates texture from image data
    void Generate(unsigned int inWidth, unsigned int inHeight, unsigned char* data);
};