#ifndef __TEXTURE_H_
#define __TEXTURE_H_

class Texture
{
public:
    unsigned int ID;
    Texture(const char *imagePath);

    void filteringParameters(unsigned int minFilter, unsigned int magFilter) const;

    void bind() const;
};

#endif