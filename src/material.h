class Material {
public:
    // Constructor
    Material(float ambient, float diffuse, float specular)
        : m_ambient(ambient), m_diffuse(diffuse), m_specular(specular) {}

    // Getters
    float getAmbient() const { return m_ambient; }
    float getDiffuse() const { return m_diffuse; }
    float getSpecular() const { return m_specular; }

    // Setters
    void setAmbient(float ambient) { m_ambient = ambient; }
    void setDiffuse(float diffuse) { m_diffuse = diffuse; }
    void setSpecular(float specular) { m_specular = specular; }

private:
    float m_ambient;
    float m_diffuse;
    float m_specular;
};
