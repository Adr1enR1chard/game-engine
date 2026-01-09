#ifndef __TEXTURE_H_
#define __TEXTURE_H_

class Texture
{
public:
    unsigned int ID;
    Texture(const char *imagePath);
    Texture() = default;
    ~Texture() = default;

    static Texture White();

    void filteringParameters(unsigned int minFilter, unsigned int magFilter) const;

    void bind() const;

private:
    Texture(unsigned int ID) : ID(ID) {};
};

#endif