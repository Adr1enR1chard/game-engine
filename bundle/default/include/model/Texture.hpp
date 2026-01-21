#ifndef __TEXTURE_H_
#define __TEXTURE_H_

class Texture
{
  public:
    Texture(const char* imagePath);
    Texture()  = default;
    ~Texture() = default;

    static Texture FromFile(const char* imagePath);
    static Texture White();
    static Texture Black();
    static Texture DefaultNormalMap();

    void filteringParameters(unsigned int minFilter, unsigned int magFilter) const;
    bool isValid() const
    {
        return ID != 0;
    }

  protected:
    friend class RenderSystem;
    unsigned int ID;

  private:
    void bind() const;

    friend class Material;

  protected:
    Texture(unsigned int ID) : ID(ID) {};
};

#endif